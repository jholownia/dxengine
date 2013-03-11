/*
================================
 app.h

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#pragma once

// This should speed up build process
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "scenemanager.h"
#include "inputmanager.h"
#include "soundmanager.h"
#include "timer.h"
#include "fpscounter.h"
#include "cpumonitor.h"
#include "position.h" 
#include "common.h"


/*
================================
 
 App

 Main application class. Create and destroys all the other objects,
 handles input and controls graphics and sound.
 
================================
*/
class App : Uncopyable
{
public:
	App(void);	
	~App(void);

	bool init();
	void shutdown();
	void run();

	LRESULT CALLBACK messageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	LPCWSTR appName_;    
	HINSTANCE hinstance_;
	HWND hwnd_;          
	InputManager* input_;
	SceneManager* scene_;           
	SoundManager* sound_;
	Timer* timer_;
	FpsCounter* fpsCounter_;
	CpuMonitor* cpuMonitor_;
	Position* cameraPosition_;
	Position* tigerPosition_;

	bool frame();
	void initializeWindows(int&, int&);
	void shutdownWindows();

};

static App* appHandle = NULL;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



