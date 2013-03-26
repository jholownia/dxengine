/*
================================
 objectlist.cpp

 Created on: 14 Jan 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#pragma once

#include "objectlist.h"

/*
================
 ObjectList::ObjectList
================
*/
ObjectList::ObjectList(void) :
	objects_ (0),
	states_  (0)
{

}

/*
================
 ObjectList::~ObjectList
================
*/
ObjectList::~ObjectList(void)
{

}

/*
================
 ObjectList::init
================
*/
bool ObjectList::init( int numObjects )
{	
	return true;
}

/*
================
 ObjectList::shutdown
================
*/
void ObjectList::shutdown()
{
	
}

/*
================
 ObjectList::getObjectCount
================
*/
int ObjectList::getObjectCount() const
{
	return objects_.size();
}

/*
================
 ObjectList::getData
================
*/
void ObjectList::getData( int index, float& x, float& y, float& z)
{
	x = objects_[index].x;
	y = objects_[index].y;
	z = objects_[index].z;
	
}

/*
================
 ObjectList::addObject

 Adds object position and physics state to the list.
================
*/
void ObjectList::addObject( float x, float y, float z, float angle)
{
	if (objects_.size() > MAX_OBJECTS)
	{
		objects_.pop_front();
		states_.pop_front();
	}	
	
	Vector3D position(x, y, z);
		
	objects_.push_back(position);
	
	// Set velocity vector to point in the same direction that tiger does
	angle += (float)D3DX_PI;		
	float velX = sinf(angle);
	float velZ = cosf(angle);	
	Vector3D velocity(velX, 0.0f, velZ);	
	velocity = velocity * 0.05f;
	
	PhysicsState state;
	state.init(velocity);

	states_.push_back(state);
}

/*
================
 ObjectList::frame

 FIXME: slow !
================
*/
void ObjectList::frame( float frameTime, Quadtree* terrain )
{
	for (unsigned int i = 0; i < objects_.size(); ++i)
	{
		states_[i].update(frameTime, terrain, objects_[i]);
	}
}
