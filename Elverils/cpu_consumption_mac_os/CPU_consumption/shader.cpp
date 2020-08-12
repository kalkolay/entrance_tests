#include "shader.h"

#include <iostream>

Shader::Shader(const GLchar* vertexPath,
               const GLchar* fragmentPath,
               GLfloat* projectionMatrix)
{
    this->projectionMatrix = projectionMatrix;
    
    const GLchar* vShaderCode = "#version 330 core\n"
    "layout (location = 0) in vec2 position;\n"
    "layout (location = 1) in vec3 color;\n"
    "uniform mat4 uProjectionMatrix;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "ourColor = color;\n"
    "gl_Position = uProjectionMatrix*(vec4(position, 0, 1.0));\n"
    "}\0";
    
    const GLchar* fShaderCode = "#version 330 core\n"
    "in vec3 ourColor;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(ourColor, 1.0f);\n"
    "}\0";
    
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->Program, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use()
    {
        glUseProgram(this->Program);

        GLuint projectionMatrixRef = glGetUniformLocation(this->Program, "uProjectionMatrix");

        glUniformMatrix4fv(projectionMatrixRef, 1, GL_FALSE, projectionMatrix);
    }
