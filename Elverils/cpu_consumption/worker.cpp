#include <stdlib.h>
#include <string>
#include <GL/freeglut.h>
#include "constants.h"
#include "worker.h"

Worker::Worker()
{
	for (int i = 0; i < STEPSINTERVAL; ++i)
		points[i] = 0;
}

void Worker::RenderString(float x, float y, void* font, const unsigned char* string)
{
	glColor3f(0, 0, 0);

	glRasterPos2f(x, y);

	glutBitmapString(font, string);
}

void Worker::draw()
{
	RenderString(area.left, HEIGHT - area.top - area.height - 10, GLUT_BITMAP_HELVETICA_18, reinterpret_cast<const unsigned char *>(title.c_str()));

	glColor3f(217.0 / 0xff, 234.0 / 0xff, 244.0 / 0xff);

	for (int i = 0; i < steps + 1; ++i)
	{
		glBegin(GL_LINES);
		glVertex2d(area.left + (area.width / steps) * i, HEIGHT - area.top);
		glVertex2d(area.left + (area.width / steps) * i, HEIGHT - area.top - area.height);
		glEnd();

		glBegin(GL_LINES);
		glVertex2d(area.left, HEIGHT - area.top - (area.height / steps) * i);
		glVertex2d(area.left + area.width, HEIGHT - area.top - (area.height / steps) * i);
		glEnd();
	}

	for (int i = 0; i < STEPSINTERVAL; ++i)
	{
		glBegin(GL_LINES);
		glColor3f(i ? points[i - 1] / 100 : points[i] / 100, 1 - points[i ? i - 1 : i] / 100, 0);

		glVertex2d(area.left + area.width / STEPSINTERVAL * (i ? i - 1 : i), HEIGHT - area.top - area.height + area.height * points[i ? i - 1 : i] / 100);

		glColor3f(i ? points[i - 1] / 100 : points[i] / 100, 1 - points[i] / 100, 0);

		glVertex2d(area.left + area.width / STEPSINTERVAL * i, HEIGHT - area.top - area.height + area.height * points[i] / 100);

		glEnd();

		if (i >= currentInterval)
			break;
	}
}

DWORD WINAPI Worker::StaticThreadStart(void* Param)
{
	Worker* This = (Worker*)Param;
	This->ThreadStart();

	return 0;
}

void  Worker::ThreadStart(void)
{
	while (true)
	{
		points[currentInterval] = getCurrentValue();

		if (currentInterval >= STEPSINTERVAL - 1)
		{
			for (int i = 1; i < STEPSINTERVAL; ++i)
				points[i - 1] = points[i];

			stopPush = true;
		}

		if (!stopPush)
			currentInterval++;

		Sleep(500);
	}
}

void Worker::start()
{
	DWORD ThreadID;
	CreateThread(NULL, 0, StaticThreadStart, (void*) this, 0, &ThreadID);
}
