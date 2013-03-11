/*
================================
 cpumonitor.h

 Created on: 17 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#pragma comment(lib, "pdh.lib")

#include <Pdh.h>

#include "common.h"

/*
================================
 
 CpuMonitor

 Measures the percentage of CPU usage across all cores.
 
================================
*/
class CpuMonitor : Uncopyable
{
public:
	CpuMonitor(void);
	~CpuMonitor(void);

	void init();
	void shutdown();
	void frame();

	int getCpuUsage() const;

private:
	bool canReadCpu_;
	HQUERY queryHandle_;
	HCOUNTER counterHandle_;
	unsigned long lastSampleTime_;
	long cpuUsage_;
};

