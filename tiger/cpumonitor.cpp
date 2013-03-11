/*
================================
 cpumonitor.cpp

 Created on: 17 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "cpumonitor.h"

/*
================
 CpuMonitor::CpuMonitor
================
*/
CpuMonitor::CpuMonitor(void)
{

}

/*
================
 CpuMonitor::~CpuMonitor
================
*/
CpuMonitor::~CpuMonitor(void)
{

}

/*
================
 CpuMonitor::init
================
*/
void CpuMonitor::init()
{
	PDH_STATUS status;
	
	canReadCpu_ = true;

	status = PdhOpenQuery(NULL, 0, &queryHandle_);
	if (status != ERROR_SUCCESS)
	{
		canReadCpu_ = false;
	}

	status = PdhAddCounter(queryHandle_, TEXT("\\Processor(_Total)\\% processor time"), 0, &counterHandle_);
	if (status != ERROR_SUCCESS)
	{
		canReadCpu_ = false;
	}

	lastSampleTime_ = GetTickCount();

	cpuUsage_ = 0;	
}

/*
================
 CpuMonitor::shutdown
================
*/
void CpuMonitor::shutdown()
{
	if (canReadCpu_)
	{
		PdhCloseQuery(queryHandle_);
	}
}

/*
================
 CpuMonitor::frame
================
*/
void CpuMonitor::frame()
{
	if (canReadCpu_)
	{
		if ((lastSampleTime_ + 1000) < GetTickCount())
		{
			PDH_FMT_COUNTERVALUE value;
			PdhCollectQueryData(queryHandle_);
			PdhGetFormattedCounterValue(counterHandle_, PDH_FMT_LONG, NULL, &value);
			cpuUsage_ = value.longValue;

			lastSampleTime_ = GetTickCount();
		}
	}
}

/*
================
 CpuMonitor::getCpuUsage
================
*/
int CpuMonitor::getCpuUsage() const
{
	if (canReadCpu_)
	{
		return (int) cpuUsage_;
	}
	
	return 0;
}
