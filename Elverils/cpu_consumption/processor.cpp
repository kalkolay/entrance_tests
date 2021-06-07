#include <string>
#include "processor.h"

#define SystemProcessorPerformanceInformation 0x8
#define SystemBasicInformation    0x0

Processor::Processor(int n, const rect& currentArea, std::string text)
{
	area = currentArea;
	title = text;
	number = n;
}

double Processor::getCurrentValue()
{
	typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION
	{
		LARGE_INTEGER IdleTime;
		LARGE_INTEGER KernelTime;
		LARGE_INTEGER UserTime;
		LARGE_INTEGER Reserved1[2];
		ULONG Reserved2;
	} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

	typedef struct _SYSTEM_BASIC_INFORMATION
    {
		ULONG Reserved;
		ULONG TimerResolution;
		ULONG PageSize;
		ULONG NumberOfPhysicalPages;
		ULONG LowestPhysicalPageNumber;
		ULONG HighestPhysicalPageNumber;
		ULONG AllocationGranularity;
		ULONG_PTR MinimumUserModeAddress;
		ULONG_PTR MaximumUserModeAddress;
		KAFFINITY ActiveProcessorsAffinityMask;
		CCHAR NumberOfProcessors;
	} SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;

	typedef DWORD(WINAPI * PNTQUERYSYSYTEMINFORMATION)(DWORD info_class, void* out, DWORD size, DWORD* out_size);
	PNTQUERYSYSYTEMINFORMATION pNtQuerySystemInformation = NULL;

	pNtQuerySystemInformation = (PNTQUERYSYSYTEMINFORMATION)GetProcAddress(GetModuleHandle(L"NTDLL.DLL"), "NtQuerySystemInformation");
	SYSTEM_BASIC_INFORMATION sbi;
	SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION * spi;

	DWORD returnlength;
	DWORD status = pNtQuerySystemInformation(SystemBasicInformation, &sbi,
		sizeof(SYSTEM_BASIC_INFORMATION), &returnlength);

	spi = new SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION[sbi.NumberOfProcessors];

	memset(spi, 0, sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION)*sbi.NumberOfProcessors);

	status = pNtQuerySystemInformation(SystemProcessorPerformanceInformation, spi,
		(sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION)*sbi.NumberOfProcessors), &returnlength);
	int numberOfCores = returnlength / sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION);

	static ULARGE_INTEGER  ul_sys_idle_old[32];
	static ULARGE_INTEGER  ul_sys_kernel_old[32];
	static ULARGE_INTEGER  ul_sys_user_old[32];

	float usage = 0;
	float usageAccum = 0;

	status = pNtQuerySystemInformation(SystemProcessorPerformanceInformation, spi,
		(sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION)*numberOfCores), &returnlength);
	usageAccum = 0;

	ULARGE_INTEGER        ul_sys_idle;
	ULARGE_INTEGER        ul_sys_kernel;
	ULARGE_INTEGER        ul_sys_user;

	ul_sys_idle.QuadPart = spi[number].IdleTime.QuadPart;
	ul_sys_kernel.QuadPart = spi[number].KernelTime.QuadPart;
	ul_sys_user.QuadPart = spi[number].UserTime.QuadPart;

	ULONGLONG kernelTime = (ul_sys_kernel.QuadPart - ul_sys_kernel_old[number].QuadPart);
	ULONGLONG usertime = (ul_sys_user.QuadPart - ul_sys_user_old[number].QuadPart);
	ULONGLONG idletime = (ul_sys_idle.QuadPart - ul_sys_idle_old[number].QuadPart);

	ULONGLONG proctime = kernelTime + usertime - idletime;

	ULONGLONG totaltime = kernelTime + usertime;

	usage = (float)(proctime * 100) / totaltime;
	usageAccum += usage;

	return usage;
}
