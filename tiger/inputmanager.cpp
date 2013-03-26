/*
================================
inputmanager.cpp

 Created on: 19 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "inputmanager.h"

/*
================
 InputManager::InputManager
================
*/
InputManager::InputManager(void) :
	directInput_ (NULL),
	keyboard_    (NULL),
	mouse_       (NULL)
{

}

/*
================
 InputManager::~InputManager
================
*/
InputManager::~InputManager(void)
{

}

/*
================
 InputManager::init
================
*/
bool InputManager::init( HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight )
{
	HRESULT result;
	screenWidth_ = screenWidth;
	screenHeight_ = screenHeight;
	mouseX_ = 0;
	mouseY_ = 0;
	mouseLastX_ = 0;
	mouseLastY_ = 0;

	// Initialize direct input
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **) &directInput_, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Setup keyboard interface
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	if (FAILED(result))
	{
		return false;
	}
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}
	result = keyboard_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);    // chaging this to nonexclusive will give other programs access to the keyboard
	if (FAILED(result))
	{
		return false;
	}
	result = keyboard_->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// Setup mouse
	result = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	if (FAILED(result))
	{
		return false;
	}
	result = mouse_->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}
	result = mouse_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}
	result = mouse_->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

/*
================
 InputManager::shutdown
================
*/
void InputManager::shutdown()
{
	if (mouse_)
	{
		mouse_->Unacquire();
		mouse_->Release();
		mouse_ = NULL;
	}

	if (keyboard_)
	{
		keyboard_->Unacquire();
		keyboard_->Release();
		keyboard_ = NULL;
	}

	if (directInput_)
	{
		directInput_->Release();
		directInput_ = NULL;
	}
}

/*
================
 InputManager::frame
================
*/
bool InputManager::frame()
{
	bool result;

	mouseLastX_ = mouseX_;
	mouseLastY_ = mouseY_;

	result = readKeyboard();
	if (!result)
	{
		return false;
	}

	result = readMouse();
	if (!result)
	{
		return false;
	}

	processInput();

	return true;
}

/*
================
 InputManager::isEscPressed
================
*/
bool InputManager::isEscPressed()
{
	if (keyboardState_[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

/*
================
 InputManager::getMousePosition
================
*/
void InputManager::getMousePosition( int& x, int& y)
{
	x = mouseX_;
	y = mouseY_;
}

/*
================
 InputManager::readKeyboard
================
*/
bool InputManager::readKeyboard()
{
	HRESULT result;

	result = keyboard_->GetDeviceState(sizeof(keyboardState_), (LPVOID) &keyboardState_);
	if(FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			keyboard_->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

/*
================
 InputManager::readMouse
================
*/
bool InputManager::readMouse()
{
	HRESULT result;

	result = mouse_->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID) &mouseState_);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			mouse_->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

/*
================
 InputManager::processInput
================
*/
void InputManager::processInput()
{
	mouseX_ += mouseState_.lX;
	mouseY_ += mouseState_.lY;
	
	/*
	if (mouseX_ < 0)
	{
		mouseX_ = 0;
	}
	if (mouseY_ < 0)
	{
		mouseY_ = 0;
	}
	if (mouseX_ > screenWidth_)
	{
		mouseX_ = screenWidth_;
	}
	if (mouseY_ > screenHeight_)
	{
		mouseY_ = screenHeight_;
	}
	*/
}

/*
================
 InputManager::isLeftArrowPressed
================
*/
bool InputManager::isLeftArrowPressed()
{
	return (keyboardState_[DIK_LEFTARROW] & 0x80);	
}

/*
================
 InputManager::isRightArrowPressed
================
*/
bool InputManager::isRightArrowPressed()
{
	return (keyboardState_[DIK_RIGHTARROW] & 0x80);
}

/*
================
 InputManager::isUpArrowPressed
================
*/
bool InputManager::isUpArrowPressed()
{
	return (keyboardState_[DIK_UPARROW] & 0x80);
}

/*
================
 InputManager::isDownArrowPressed
================
*/
bool InputManager::isDownArrowPressed()
{
	return (keyboardState_[DIK_DOWNARROW] & 0x80);
}

/*
================
 InputManager::isWPressed
================
*/
bool InputManager::isWPressed()
{
	return (keyboardState_[DIK_W] & 0x80);
}

/*
================
 InputManager::isAPressed
================
*/
bool InputManager::isAPressed()
{
	return (keyboardState_[DIK_A] & 0x80);	
}

/*
================
 InputManager::isSPressed
================
*/
bool InputManager::isSPressed()
{
	return (keyboardState_[DIK_S] & 0x80);	
}

/*
================
 InputManager::isDPressed
================
*/
bool InputManager::isDPressed()
{
	return (keyboardState_[DIK_D] & 0x80);	
}

/*
================
 InputManager::isSpacePressed
================
*/
bool InputManager::isSpacePressed()
{
	return (keyboardState_[DIK_SPACE] & 0x80);
}

/*
================
 InputManager::isKeyDown
================
*/
bool InputManager::isKeyDown( unsigned int key )
{
	return (keyboardState_[key] & 0x80);
}

void InputManager::getMouseDelta( int& dx, int& dy)
{
	dx = mouseX_ - mouseLastX_;
	dy = mouseY_ - mouseLastY_;
}
