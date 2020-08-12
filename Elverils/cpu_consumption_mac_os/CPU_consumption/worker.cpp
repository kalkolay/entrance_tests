#include "worker.h"

#include <unistd.h>
#include <pthread.h>

Worker::Worker()
{
	for (size_t i = 0; i < STEPSINTERVAL; ++i)
		points[i] = 0;
}

void Worker::InitializeShaderData()
{
	countGridPoints = (steps + 1) * 4;
	GLsizeiptr size = (countGridPoints + STEPSINTERVAL) * coordOnPoint * sizeof(GLfloat);
	GLfloat* vertices = new GLfloat[(countGridPoints + STEPSINTERVAL) * coordOnPoint]();

	GLfloat gridColor[3] =
    {
		217.0 / 0xff, 234.0 / 0xff, 244.0 / 0xff
	};

	bool direction = false;

	for (int i = 0, k = 0; i < countGridPoints; i += 2, k += 2)
	{
		if (k % ((steps + 1) * 2) == 0)
			k = 0;

		if ( (int)(i / ((steps + 1) * 2)) == 0 )
		{
			vertices[i * coordOnPoint] = area.left + (area.width / steps) * (int)(i / 2);
			vertices[(i + (int)direction) * coordOnPoint + 1] = area.top + area.height;

			vertices[(i + 1) * coordOnPoint] = area.left + (area.width / steps) * (int)((i + 1) / 2);
			vertices[(i + (int)(!direction)) * coordOnPoint + 1] = area.top;
		}
		else
		{
			vertices[(i + (int)(direction)) * coordOnPoint] = area.left;
			vertices[i * coordOnPoint + 1] = area.top + (area.height / steps) * (int)(k / 2);

			vertices[(i + (int)(!direction)) * coordOnPoint] = area.left + area.width;
			vertices[(i + 1) * coordOnPoint + 1] = area.top + (area.height / steps) * (int)((k + 1) / 2);
		}

		vertices[i * coordOnPoint + 2] = gridColor[0];
		vertices[i * coordOnPoint + 3] = gridColor[1];
		vertices[i * coordOnPoint + 4] = gridColor[2];

		vertices[(i + 1) * coordOnPoint + 2] = gridColor[0];
		vertices[(i + 1) * coordOnPoint + 3] = gridColor[1];
		vertices[(i + 1) * coordOnPoint + 4] = gridColor[2];

		direction = !direction;
	}

	for (size_t i = 0; i < STEPSINTERVAL; ++i)
	{
		vertices[(countGridPoints + i) * coordOnPoint] = area.left + (area.width / (STEPSINTERVAL - 1)) * i;
		vertices[(countGridPoints + i) * coordOnPoint + 4] = 0.0f;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STREAM_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, coordOnPoint * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, coordOnPoint * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Worker::RenderString(float x, float y, void* font, const unsigned char* string)
{
	glColor3f(0, 0, 0);

	glRasterPos2f(x, y);
}

void Worker::draw(Shader* shader)
{
	Update();

	shader->Use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, countGridPoints);
	glDrawArrays(GL_LINE_STRIP, countGridPoints, currentInterval);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Worker::Update()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	GLfloat* mapBuffer = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	for (size_t i = 0; i < currentInterval; ++i)
	{
		mapBuffer[(i + countGridPoints) * coordOnPoint + 1] = area.top + (1.0f - (points[i]) / 100.0f) * area.height;
		mapBuffer[(i + countGridPoints) * coordOnPoint + 2] = points[i] / 100.0f;
		mapBuffer[(i + countGridPoints) * coordOnPoint + 3] = 1.0f - points[i] / 100.0f;
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void* Worker::StaticThreadStart(void* Param)
{
	Worker* This = (Worker*)Param;
	This->ThreadStart();

	return 0;
}

void Worker::ThreadStart(void)
{
	while (true)
	{
		if (!stopPush)
			currentInterval++;

		if (currentInterval > STEPSINTERVAL - 1)
		{
			for (size_t i = 1; i < STEPSINTERVAL; ++i)
				points[i - 1] = points[i];

			stopPush = true;
		}

		points[currentInterval - 1] = getCurrentValue();
		
		usleep(500000);
	}
}

void Worker::start()
{
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, StaticThreadStart, (void*) this);
}
