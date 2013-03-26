/*
================================
 inputmanager.h

 Created on: 19 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <array>

#include "common.h"


/*
================================
 
 InputManager

 This class uses DirectInput for handling user input from
 keyboard and mouse.
 
================================
*/
class InputManager : Uncopyable
{
public:
	InputManager(void);	
	~InputManager(void);

	bool init(HINSTANCE, HWND, int, int);
	void shutdown();
	bool frame();

	// Convenience methods
	bool isEscPressed();
	bool isLeftArrowPressed();
	bool isRightArrowPressed();
	bool isUpArrowPressed();
	bool isDownArrowPressed();
	bool isWPressed();
	bool isAPressed();
	bool isSPressed();
	bool isDPressed();
	bool isSpacePressed();

	void getMousePosition(int&, int&);
	void getMouseDelta(int&, int&);	
	
	bool isKeyDown(unsigned int);

private:
	IDirectInput8* directInput_;
	IDirectInputDevice8* keyboard_;
	IDirectInputDevice8* mouse_;
	std::array<unsigned char, 256> keyboardState_;
	DIMOUSESTATE mouseState_;
	int screenWidth_;
	int screenHeight_;
	int mouseX_;
	int mouseLastX_;
	int mouseY_;
	int mouseLastY_;

	bool readKeyboard();
	bool readMouse();
	void processInput();
};

