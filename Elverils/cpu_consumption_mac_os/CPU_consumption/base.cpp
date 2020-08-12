#include "processor.h"

#include <mach/mach.h>

Base::Base(const rect& currentArea, std::string text)
{
	area = currentArea;
	title = text;

	InitializeShaderData();
}

double Base::getCurrentValue()
{
    host_cpu_load_info_data_t cpuInfo;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuInfo, &count) == KERN_SUCCESS)
    {
        unsigned long long totalTicks = 0;
        for (size_t i = 0; i < CPU_STATE_MAX; ++i)
            totalTicks += cpuInfo.cpu_ticks[i];
        return calculateTotalConsumption(cpuInfo.cpu_ticks[CPU_STATE_IDLE], totalTicks);
    }
    else
        return -1.0f;
}

float Base::calculateTotalConsumption(unsigned long long idleTicks, unsigned long long totalTicks)
{
    unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
    unsigned long long idleTicksSinceLastTime  = idleTicks  - _previousIdleTicks;
    float ans = 1.0f -
                (totalTicksSinceLastTime > 0 ? static_cast<float>(idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);
    _previousTotalTicks = totalTicks;
    _previousIdleTicks  = idleTicks;
    return ans * 100;
}
