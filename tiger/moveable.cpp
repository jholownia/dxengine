/*
================================
 moveable.cpp

 Created on: 29 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "moveable.h"

/*
================
 Moveable::Moveable
================
*/
Moveable::Moveable(void) :
	x_  (0.0f),
	y_  (0.0f),
	z_  (0.0f),
	rx_ (0.0f),
	ry_ (0.0f),
	rz_ (0.0f)	
{

}

/*
================
 Moveable::~Moveable
================
*/
Moveable::~Moveable(void)
{

}

/*
================
 Moveable::setPosition
================
*/
void Moveable::setPosition( float x, float y, float z )
{
	x_ = x;
	y_ = y;
	z_ = z;
}

/*
================
 Moveable::setRotation
================
*/
void Moveable::setRotation( float rx, float ry, float rz )
{
	rx_ = rx;
	ry_ = ry;
	rz_ = rz;
}

/*
================
 Moveable::getPosition
================
*/
D3DXVECTOR3 Moveable::getPosition() const
{
	return D3DXVECTOR3(x_, y_, z_);
}

/*
================
 Moveable::getRotation
================
*/
D3DXVECTOR3 Moveable::getRotation() const
{
	return D3DXVECTOR3(rx_, ry_, rz_);
}
