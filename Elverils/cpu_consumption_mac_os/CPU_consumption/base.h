#pragma once

#include <cstring>

class Base : public Worker
{
public:
	Base(const struct rect&, std::string);
    double getCurrentValue();

private:
    unsigned long long _previousTotalTicks = 0;
    unsigned long long _previousIdleTicks  = 0;
    
    float calculateTotalConsumption(unsigned long long, unsigned long long);
};

