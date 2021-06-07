#pragma once

#include "worker.h"
#include "base.h"

#include <mach/mach.h>

class Processor : public Worker
{
typedef struct _coreinfo_t
{
    natural_t cpu_ticks[CPU_STATE_MAX];
    double usage;
} coreinfo_t;
    
public:
    Processor(int, const rect&, std::string);
    ~Processor();
    void update();
    double getCurrentValue();
    
private:
    unsigned core_count;
    coreinfo_t* cores;
    int number;
};
