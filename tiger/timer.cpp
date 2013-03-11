/*
================================
 timer.cpp

 Created on: 11 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "timer.h"

/*
================
 Timer::Timer
================
*/
Timer::Timer(void)
{
}

/*
================
 Timer::~Timer
================
*/
Timer::~Timer(void)
{
}

/*
================
 Timer::init
================
*/
bool Timer::init()
{
	QueryPerformanceFrequency((LARGE_INTEGER *) &frequency_);
	if (frequency_ == 0)
	{
		return false;
	}

	ticksPerMs_ = (float)(frequency_ / 1000);

	QueryPerformanceCounter((LARGE_INTEGER *) &startTime_);

	return true;
}

/*
================
 Timer::frame
================
*/
void Timer::frame()
{
	INT64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER *) &currentTime);
	float timeDifference = (float) (currentTime - startTime_);
	frameTime_ = timeDifference / ticksPerMs_;
	startTime_ = currentTime;
}

/*
================
 Timer::getTime
================
*/
float Timer::getTime()
{
	return frameTime_;
}
