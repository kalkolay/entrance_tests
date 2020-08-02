#include <string>
#include "windows.h"
#include "constants.h"
#include "container.h"

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
	void draw();
	void start();
	static DWORD WINAPI StaticThreadStart(void* Param);
	void ThreadStart();
	virtual double getCurrentValue() = 0;
	rect area;
	std::string title;

private:
	int steps = 10;
	int padding = 10;
	int currentInterval = 0;
	bool stopPush = false;
	void RenderString(float x, float y, void* font, const unsigned char* string);

	Array<double, STEPSINTERVAL> points{};
};
