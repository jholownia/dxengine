/*
================================
 position.h

 Created on: 14 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "position.h"

/*
================
 Position::Position
================
*/
Position::Position(void) :
	posX_             (0.0f),
	posY_             (0.0f),
	posZ_             (0.0f),
	rotationX_        (0.0f),
	rotationY_        (0.0f),
	rotationZ_        (0.0f),
	frameTime_        (0.0f),	
	leftTurnSpeed_    (0.0f),
	rightTurnSpeed_   (0.0f),
	forwardSpeed_     (0.0f),
	backwardSpeed_    (0.0f),
	strafeRightSpeed_ (0.0f),
	strafeLeftSpeed_  (0.0f)
{
	
}

/*
================
 Position::~Position
================
*/
Position::~Position(void)
{

}

/*
================
 Position::setFrameTime
================
*/
void Position::setFrameTime( float time )
{
	frameTime_ = time;
}

/*
================
 Position::turnLeft
================
*/
void Position::turnLeft( bool keydown )
{
	if (keydown)
	{
		leftTurnSpeed_ += frameTime_ * 0.01f;

		if (leftTurnSpeed_ > (frameTime_ * 0.15f))
		{
			leftTurnSpeed_ = frameTime_ * 0.15f;
		}
	}
	else
	{	
		leftTurnSpeed_ = frameTime_ * 0.005f;

		if (leftTurnSpeed_ < 0.0f)
		{
			leftTurnSpeed_ = 0.0f;
		}		
	}

	rotationY_ -= leftTurnSpeed_;
	if (rotationY_ < 0.0f)
	{
		rotationY_ += 360.0f;
	}

}

/*
================
 Position::turnRight
================
*/
void Position::turnRight( bool keydown )
{
	if (keydown)
	{
		rightTurnSpeed_ += frameTime_ * 0.01f;

		if (rightTurnSpeed_ > (frameTime_ * 0.15f))
		{
			rightTurnSpeed_ = frameTime_ * 0.15f;
		}
	}
	else
	{	
		rightTurnSpeed_ = frameTime_ * 0.005f;

		if (rightTurnSpeed_ < 0.0f)
		{
			rightTurnSpeed_ = 0.0f;
		}		
	}

	rotationY_ += rightTurnSpeed_;
	if (rotationY_ > 360.0f)
	{
		rotationY_ -= 360.0f;
	}
}

/*
================
 Position::setPosition
================
*/
void Position::setPosition( float x, float y, float z )
{
	posX_ = x;
	posY_ = y;
	posZ_ = z;
}

/*
================
 Position::setRotation
================
*/
void Position::setRotation( float x, float y, float z )
{
	rotationX_ = x;
	rotationY_ = y;
	rotationZ_ = z;
}

/*
================
 Position::getPosition
================
*/
void Position::getPosition( float& x, float& y, float& z )
{
	x = posX_;
	y = posY_;
	z = posZ_;
}

/*
================
 Position::getRotation
================
*/
void Position::getRotation( float& x, float& y, float& z )
{
	x = rotationX_;
	y = rotationY_;
	z = rotationZ_;
}

/*
================
 Position::moveForward
================
*/
void Position::moveForward( bool keydown )
{
	if (keydown)
	{
		forwardSpeed_ += frameTime_ * 0.001f;

		if (forwardSpeed_ > (frameTime_ * 0.03f))
		{
			forwardSpeed_ = frameTime_ * 0.03f;
		}
	}
	else
	{
		forwardSpeed_ -= frameTime_ * 0.0007f;

		if (forwardSpeed_ < 0.0f)
		{
			forwardSpeed_ = 0.0f;
		}
	}

	float radians = rotationY_ * 0.0174532925f;
	float radiansz = rotationX_ * 0.0174532925f;

	posX_ += sinf(radians) * forwardSpeed_;
	posY_ += -sinf(radiansz) * forwardSpeed_;
	posZ_ += cosf(radians) * forwardSpeed_;
}

/*
================
 Position::moveBack
================
*/
void Position::moveBack( bool keydown )
{
	if (keydown)
	{
		backwardSpeed_ += frameTime_ * 0.001f;

		if (backwardSpeed_ > (frameTime_ * 0.03f))
		{
			backwardSpeed_ = frameTime_ * 0.03f;
		}
	}
	else
	{
		backwardSpeed_ -= frameTime_ * 0.0007f;

		if (backwardSpeed_ < 0.0f)
		{
			backwardSpeed_ = 0.0f;
		}
	}

	float radians = rotationY_ * 0.0174532925f;
	float radiansz = rotationX_ * 0.0174532925f;

	posX_ -= sinf(radians) * backwardSpeed_;
	posY_ -= -sinf(radiansz) * backwardSpeed_;
	posZ_ -= cosf(radians) * backwardSpeed_;
}

/*
================
 Position::strafeLeft
================
*/
void Position::strafeLeft( bool keydown )
{
	if (keydown)
	{
		strafeLeftSpeed_ += frameTime_ * 0.001f;

		if (strafeLeftSpeed_ > (frameTime_ * 0.03f))
		{
			strafeLeftSpeed_ = frameTime_ * 0.03f;
		}
	}
	else
	{
		strafeLeftSpeed_ -= frameTime_ * 0.0007f;

		if (strafeLeftSpeed_ < 0.0f)
		{
			strafeLeftSpeed_ = 0.0f;
		}
	}

	float radians = (rotationY_ + 90) * 0.0174532925f;

	posX_ -= sinf(radians) * strafeLeftSpeed_;
	posZ_ -= cosf(radians) * strafeLeftSpeed_;
}

/*
================
 Position::strafeRight
================
*/
void Position::strafeRight( bool keydown )
{
	if(keydown)
	{
		strafeRightSpeed_ += frameTime_ * 0.001f;

		if(strafeRightSpeed_ > (frameTime_ * 0.03f))
		{
			strafeRightSpeed_ = frameTime_ * 0.03f;
		}
	}
	else
	{
		strafeRightSpeed_ -= frameTime_ * 0.0007f;

		if(strafeRightSpeed_ < 0.0f)
		{
			strafeRightSpeed_ = 0.0f;
		}
	}

	// Convert degrees to radians.
	float radians = (rotationY_ + 90) * 0.0174532925f;

	// Update the position.
	posX_ += sinf(radians) * strafeRightSpeed_;
	posZ_ += cosf(radians) * strafeRightSpeed_;
}

/*
================
 Position::getYRotation
================
*/
void Position::getYRotation( float& rotation)
{
	rotation = rotationY_;
}
