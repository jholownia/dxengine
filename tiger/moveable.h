/*
================================
 moveable.h
 
 Created on: 29 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <D3DX10math.h>


/*
================================
 
 Moveable

 Provides a common interface for all moveable objects.
 
================================
*/
class Moveable
{
public:
	Moveable(void);
	virtual ~Moveable(void);

	void setPosition(float x, float y, float z);
	void setRotation(float rx, float ry, float rz);

	D3DXVECTOR3 getPosition() const;
	D3DXVECTOR3 getRotation() const;

protected:
	float x_;
	float y_;
	float z_;
	float rx_;
	float ry_;
	float rz_;
};

