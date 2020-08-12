#pragma once

#include "worker.h"
#include "base.h"

#include <vector>

#include <mach/mach.h>

static processor_cpu_load_info_t cpuInfo, prevCpuInfo;
static mach_msg_type_number_t numCpuInfo, numPrevCpuInfo;

inline std::vector<double> calculateCoreConsumption()
{
    float inUse, total = 0.0f;
    unsigned numCPUs;

    host_processor_info(mach_host_self(), PROCESSOR_CPU_LOAD_INFO, &numCPUs, (processor_info_array_t*)&cpuInfo, &numCpuInfo);
    
    std::vector<double> ans(numCPUs);
    for (size_t i = 0; i < numCPUs; ++i)
    {
        if (prevCpuInfo)
        {
            inUse = (
                     (cpuInfo[i].cpu_ticks[CPU_STATE_USER]   - prevCpuInfo[i].cpu_ticks[CPU_STATE_USER]) +
                     (cpuInfo[i].cpu_ticks[CPU_STATE_SYSTEM] - prevCpuInfo[i].cpu_ticks[CPU_STATE_SYSTEM]) +
                     (cpuInfo[i].cpu_ticks[CPU_STATE_NICE]   - prevCpuInfo[i].cpu_ticks[CPU_STATE_NICE])
                    );
            total = inUse + (cpuInfo[i].cpu_ticks[CPU_STATE_IDLE] - prevCpuInfo[i].cpu_ticks[CPU_STATE_IDLE]);
        }
        else
        {
            inUse = cpuInfo[i].cpu_ticks[CPU_STATE_USER] + cpuInfo[i].cpu_ticks[CPU_STATE_SYSTEM] + cpuInfo[i].cpu_ticks[CPU_STATE_NICE];
            total = inUse + cpuInfo[i].cpu_ticks[CPU_STATE_IDLE];
        }
        
        if (total != 0)
            ans[i] = 100 * inUse / total;
    }

    if (prevCpuInfo)
    {
        size_t prevCpuInfoSize = sizeof(integer_t) * numPrevCpuInfo;
        vm_deallocate(mach_task_self(), (vm_address_t)prevCpuInfo, prevCpuInfoSize);
    }
    prevCpuInfo = cpuInfo;
    numPrevCpuInfo = numCpuInfo;

    cpuInfo = nullptr;
    numCpuInfo = 0;
    
    return ans;
}

class Processor : public Worker
{
public:
	Processor(int, const std::vector<double>&, const struct rect&, std::string);
    
    double getCurrentValue();

private:
	int number;
    std::vector<double> cores;
};
