/*
================================
 objectlist.h

 Created on: 14 Jan 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <d3dx10math.h>
#include <stdlib.h>
#include <time.h>

#include <deque>
#include "vector3d.h"
#include "physicsstate.h"
#include "quadtree.h"
#include "common.h"

const size_t MAX_OBJECTS = 10;
/*
================================
 
 ObjectList

 This class provides a container to for storing objects' positions
 and physics states. Work in progress.
 
================================
*/
class ObjectList : Uncopyable
{
public:
	ObjectList(void);
	~ObjectList(void);

	bool init(int);
	void shutdown();
	void frame(float frameTime, Quadtree* terrain);

	int getObjectCount() const;
	void getData(int, float&, float&, float&);

	void addObject(float, float, float, float);

private:
	std::deque<Vector3D> objects_;
	std::deque<PhysicsState> states_;
};

