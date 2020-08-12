#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

uniform mat4 uProjectionMatrix;

out vec3 ourColor;

void main()
{
    ourColor = color;

	gl_Position = uProjectionMatrix*(vec4(position, 0, 1.0));

}
