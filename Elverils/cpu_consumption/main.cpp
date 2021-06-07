#include <stdlib.h>
#include <windows.h>
#include <sstream>
#include <GL/freeglut.h>
#include "constants.h"
#include "processor.h"
#include "stdio.h"

Base* graph;
Processor** graph2;
int numCPU;

static void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 300, 0.0, 300);

	graph = new Base({ 100, 100, 10, 10 }, "Total CPU load");
	graph->start();

	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	numCPU = sysinfo.dwNumberOfProcessors;

	graph2 = new Processor*[numCPU];

	for (int i = 0; i < numCPU; ++i)
	{
		std::ostringstream title;
		title << "Core " << i + 1;

		graph2[i] = new Processor(i, { 50, 50, (double)(120 + i % 2 * 60), (double)(i / 2 * 70 + 10)}, title.str());
		graph2[i]->start();
	}
}

static void Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glPointSize(3.0);

	graph->draw();

	for (int i = 0; i < numCPU; ++i)
		graph2[i]->draw();

	glFlush();
}

void redraw(int time)
{
	glutPostRedisplay();
	glutTimerFunc(UPDATE, redraw, 0);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	glutInit(&__argc, __argv);

	glutInitWindowSize(800, 500);

	glutCreateWindow("CPU consumption");

	Init();
	glutDisplayFunc(Draw);
	glutTimerFunc(UPDATE, redraw, 0);
	glutMainLoop();

	return 0;
}
