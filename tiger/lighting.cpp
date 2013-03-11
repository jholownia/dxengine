/*
================================
 lighting.cpp

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "lighting.h"

/*
================
 Lighting::Lighting
================
*/
Lighting::Lighting(void)
{

}

/*
================
 Lighting::~Lighting
================
*/
Lighting::~Lighting(void)
{

}

/*
================
 Lighting::setDiffuseColor
================
*/
void Lighting::setDiffuseColor( float red, float green, float blue, float alpha )
{
	diffuseColor_ = D3DXVECTOR4(red, green, blue, alpha);
}

/*
================
 Lighting::setDirection
================
*/
void Lighting::setDirection( float x, float y, float z)
{
	direction_ = D3DXVECTOR3(x, y, z);
}

/*
================
 Lighting::getDiffuseColor
================
*/
D3DXVECTOR4 Lighting::getDiffuseColor() const
{
	return diffuseColor_;
}

/*
================
 Lighting::getDirection
================
*/
D3DXVECTOR3 Lighting::getDirection() const
{
	return direction_;
}

/*
================
 Lighting::getAmbientColor
================
*/
D3DXVECTOR4 Lighting::getAmbientColor() const
{
	return ambientColor_;
}

/*
================
 Lighting::setAmbientColor
================
*/
void Lighting::setAmbientColor( float red, float green, float blue, float alpha )
{
	ambientColor_ = D3DXVECTOR4(red, green, blue, alpha);
}


