/*
================================
 timer.h

 Created on: 11 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <Windows.h>

/*
================================
 
 Timer

 High precision timer class.
 
================================
*/
class Timer
{
public:
	Timer(void);
	~Timer(void);

	bool init();
	void frame();
	float getTime();

private:
	INT64 frequency_;
	float ticksPerMs_;
	INT64 startTime_;
	float frameTime_;
};

