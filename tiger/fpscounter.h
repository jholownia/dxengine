/*
================================
 fpscounter.h
 
 Created on: 17 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <MMSystem.h>

#include "common.h"
/*
================================
 
 FpsCounter

 Counts the number of frames rendered each second.
 
================================
*/
class FpsCounter : Uncopyable
{
public:
	FpsCounter(void);
	~FpsCounter(void);

	void init();
	void frame();
	int getFps();

private:
	int fps_;
	int count_;
	unsigned long startTime_;
};

