#include <string>
#include "pdh.h"

static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

class Base : public Worker
{
public:
	Base(const struct rect&, std::string);
	double getCurrentValue();
};
