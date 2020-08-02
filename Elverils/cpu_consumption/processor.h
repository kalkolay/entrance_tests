#include <string>
#include "worker.h"
#include "base.h"


class Processor : public Worker
{
public:
	Processor(int, const struct rect&, std::string);
    ~Processor() = default;
	double getCurrentValue();

private:
	int number;
};
