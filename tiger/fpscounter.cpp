/*
================================
 fpscounter.h

 Created on: 17 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "fpscounter.h"

/*
================
 FpsCounter::FpsCounter
================
*/
FpsCounter::FpsCounter(void)
{

}

/*
================
 FpsCounter::~FpsCounter
================
*/
FpsCounter::~FpsCounter(void)
{

}

/*
================
 FpsCounter::init
================
*/
void FpsCounter::init()
{
	fps_ = 0;
	count_ = 0;
	startTime_ = timeGetTime();
}

/*
================
 FpsCounter::frame

 Called each frame.
================
*/
void FpsCounter::frame()
{
	++count_;

	// If 1 second has passed count frames since last time
	if (timeGetTime() >= (startTime_ + 1000))
	{
		fps_ = count_;
		count_ = 0;

		startTime_ = timeGetTime();
	}
}

/*
================
 FpsCounter::getFps
================
*/
int FpsCounter::getFps()
{
	return fps_;
}
