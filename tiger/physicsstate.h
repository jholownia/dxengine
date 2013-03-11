/*
================================
 physicsstate.h

 Created on: 14 Jan 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#pragma once

#include "vector3d.h"
#include "quadtree.h"


/*
================================
 
 PhysicsState

 An attempt at physics state class.
 
================================
*/
class PhysicsState
{
public:
	PhysicsState(void);
	~PhysicsState(void);

	void init(Vector3D volecity);
	void update(float, Quadtree*, Vector3D&);
	Vector3D velocity_;	
	Vector3D gravity_;
	float friction_;
};

