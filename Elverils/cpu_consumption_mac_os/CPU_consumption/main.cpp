#include "processor.h"
#include "shader.h"

#include <sstream>
#include <thread>

#include <GLFW/glfw3.h>

Base* graph;
Processor** graph2;
int numCPU;
Shader* shader;

float projectionMatrix[16];

void Ortho2DProjection(float* mat)
{
	// this is basically from
	const float zNear = -1.0f;
	const float zFar = 1.0f;
	const float inv_z = 1.0f / (zFar - zNear);
	const float inv_y = - 1.0f / HEIGHT;
	const float inv_x = 1.0f / WIDTH;

	//first column
	*mat++ = 2.0f * inv_x;
	*mat++ = 0.0f;
	*mat++ = 0.0f;
	*mat++ = 0.0f;

	//second
	*mat++ = 0.0f;
	*mat++ = 2.0 * inv_y;
	*mat++ = 0.0f;
	*mat++ = 0.0f;

	//third
	*mat++ = 0.0f;
	*mat++ = 0.0f;
	*mat++ = - 2.0f * inv_z;
	*mat++ = 0.0f;

	//fourth
	*mat++ = - WIDTH * inv_x;
	*mat++ = - HEIGHT * inv_y;
	*mat++ = - (zFar + zNear) * inv_z;
	*mat++ = 1.0f;
}

static void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);

	Ortho2DProjection(projectionMatrix);

	shader = new Shader("core.vs", "core.frag", projectionMatrix);

	graph = new Base({ 100, 100, 10, 10 }, "Total CPU load");
	graph->start();

	numCPU = std::thread::hardware_concurrency();

	graph2 = new Processor*[numCPU];

	for (int i = 0; i < numCPU; ++i)
	{
		std::ostringstream title;
		title << "Core " << i + 1;

		graph2[i] = new Processor( i, {50, 50, (double)(120 + i % 2 * 60), (double)(i / 2 * 70 + 10)}, title.str() );
		graph2[i]->start();
	}
}

static void Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glPointSize(3.0);

	graph->draw(shader);

	for (size_t i = 0; i < numCPU; ++i)
		graph2[i]->draw(shader);

	glFlush();
}


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 500, "CPU consumption", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
		return EXIT_FAILURE;

	Init();

	while ( !glfwWindowShouldClose(window) )
	{
		glfwPollEvents();
        
		Draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}
