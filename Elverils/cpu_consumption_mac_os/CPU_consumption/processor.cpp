#include "processor.h"

Processor::Processor(int n, const rect& currentArea, std::string text)
{
    mach_msg_type_number_t count;
    processor_info_array_t coreinfo;
    host_processor_info(mach_host_self(),
                        PROCESSOR_CPU_LOAD_INFO,
                        &this->core_count,
                        &coreinfo,
                        &count
                        );
    
    area = currentArea;
    title = text;
    number = n;
    cores = (coreinfo_t*)malloc(sizeof(coreinfo_t) * this->core_count);
    
    InitializeShaderData();
}

Processor::~Processor()
{
    free(cores);
}

double Processor::getCurrentValue()
{
    mach_msg_type_number_t count;
    unsigned core_count;
    processor_cpu_load_info_t coreinfo;
    natural_t user, system, idle, nice;
    double used, total;
    
    host_processor_info(mach_host_self(),
                        PROCESSOR_CPU_LOAD_INFO,
                        &core_count,
                        (processor_info_array_t*)&coreinfo,
                        &count
                        );
    user = coreinfo[number].cpu_ticks[CPU_STATE_USER] - cores[number].cpu_ticks[CPU_STATE_USER];
    system = coreinfo[number].cpu_ticks[CPU_STATE_SYSTEM] - cores[number].cpu_ticks[CPU_STATE_SYSTEM];
    idle = coreinfo[number].cpu_ticks[CPU_STATE_IDLE] - cores[number].cpu_ticks[CPU_STATE_IDLE];
    nice = coreinfo[number].cpu_ticks[CPU_STATE_NICE] - cores[number].cpu_ticks[CPU_STATE_NICE];
    used = user + system + nice;
    total = system + user + idle + nice;
    cores[number].usage = used / total;
    cores[number].cpu_ticks[CPU_STATE_USER] = coreinfo[number].cpu_ticks[CPU_STATE_USER];
    cores[number].cpu_ticks[CPU_STATE_SYSTEM] = coreinfo[number].cpu_ticks[CPU_STATE_SYSTEM];
    cores[number].cpu_ticks[CPU_STATE_IDLE] = coreinfo[number].cpu_ticks[CPU_STATE_IDLE];
    cores[number].cpu_ticks[CPU_STATE_NICE] = coreinfo[number].cpu_ticks[CPU_STATE_NICE];
    
    return cores[number].usage > 1 ? 100 : cores[number].usage * 100;
}
