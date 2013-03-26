/*
================================
 physicsstate.cpp

 Created on: 14 Jan 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#include "physicsstate.h"


/*
================
 PhysicsState::PhysicsState
================
*/
PhysicsState::PhysicsState(void)
{

}

/*
================
 PhysicsState::~PhysicsState
================
*/
PhysicsState::~PhysicsState(void)
{

}

/*
================
 PhysicsState::init
================
*/
void PhysicsState::init( Vector3D velocity )
{
	velocity_ = velocity;	
	gravity_.x = 0.0f;
	gravity_.y = -0.0005f;
	gravity_.z = 0.0f;
	friction_ = 0.001f;
}

/*
================
 PhysicsState::update
================
*/
void PhysicsState::update( float deltaTime, Quadtree* terrain, Vector3D& position )
{		
	velocity_ = velocity_ + gravity_ * deltaTime;
	velocity_.x *= 1 - (friction_ * deltaTime);
	velocity_.y *= 1 - (friction_ * deltaTime);
	velocity_.z *= 1 - (friction_ * deltaTime);
	position = position + (velocity_ * deltaTime);
	
	float height;
	if (terrain->getHeightAtPosition(position.x, position.z, height))
	{
		if (position.y < (height + 1.0f))
		{
			velocity_.y = -velocity_.y * 0.9f;
			position.y = height + 1.0f;
		}
	}
}
