#include "processor.h"

Processor::Processor(int n, const std::vector<double>& cores, const rect& currentArea, std::string text) : cores(cores)
{
	area = currentArea;
	title = text;
	number = n;

	InitializeShaderData();
}

double Processor::getCurrentValue()
{
    return cores[number];
}
