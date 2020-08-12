#pragma once

#include <GL/glew.h>

class Shader
{
public:
    GLuint Program;
	GLfloat* projectionMatrix;
    Shader(const GLchar*, const GLchar*, GLfloat*);

    void Use();
};
