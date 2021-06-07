#pragma once

#include "constants.h"
#include "container.h"
#include "shader.h"

#include <string>

class Worker
{
public:
	struct rect
	{
		double width;
		double height;
		double left;
		double top;
	};

	Worker();
	void draw(Shader*);
	void start();
	static void* StaticThreadStart(void*);
	void ThreadStart();
	virtual double getCurrentValue() = 0;
	rect area;
	std::string title;
	void InitializeShaderData();

private:
	GLfloat* vertices;
	bool initialized = false;
	int countGridPoints;
	int coordOnPoint = 5;
	int steps = 10;
	int padding = 10;
	int currentInterval = 0;
	bool stopPush = false;
	GLuint VBO;
	GLuint VAO;
	void RenderString(float, float, void*, const unsigned char*);
	void Update();
	Array<double, STEPSINTERVAL> points{};
};
