#include "Shader.h"

Shader::Shader(const GLchar* vertexPath,
               const GLchar* fragmentPath)
        : _vertexPath  (vertexPath)
        , _fragmentPath(fragmentPath)
        , _program     (0)
{}

GLuint Shader::getProgram() const
{
    return _program;
}

void Shader::init()
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    // ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions ( std::ifstream::badbit );
    fShaderFile.exceptions ( std::ifstream::badbit );
    try
    {
        // Open files
        vShaderFile.open( _vertexPath );
        fShaderFile.open( _fragmentPath );
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf( );
        fShaderStream << fShaderFile.rdbuf( );
        // close file handlers
        vShaderFile.close( );
        fShaderFile.close( );
        // Convert stream into string
        vertexCode = vShaderStream.str( );
        fragmentCode = fShaderStream.str( );
    }
    catch ( std::ifstream::failure& e )
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str( );
    const GLchar *fShaderCode = fragmentCode.c_str( );
    // 2. Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    // Vertex Shader
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, nullptr );
    glCompileShader( vertex );
    // Print compile errors if any
    glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertex, 512, nullptr, infoLog );
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment Shader
    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, nullptr );
    glCompileShader( fragment );
    // Print compile errors if any
    glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( fragment, 512, nullptr, infoLog );
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Shader Program
    _program = glCreateProgram( );
    glAttachShader( _program, vertex );
    glAttachShader( _program, fragment );
    glLinkProgram( _program );
    // Print linking errors if any
    glGetProgramiv( _program, GL_LINK_STATUS, &success );
    if (!success)
    {
        glGetProgramInfoLog( _program, 512, nullptr, infoLog );
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader( vertex );
    glDeleteShader( fragment );
}

void Shader::use() const
{
    glUseProgram( _program );
}