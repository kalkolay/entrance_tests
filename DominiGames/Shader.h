#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

class Shader
{
public:
    Shader(const GLchar* vertexPath,
           const GLchar* fragmentPath);

    [[nodiscard]] GLuint getProgram() const;

    void init();

    // Uses the current shader
    void use() const;

private:
    const GLchar* _vertexPath;
    const GLchar* _fragmentPath;
    GLuint        _program;
};
