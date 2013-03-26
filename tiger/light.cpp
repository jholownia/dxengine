/*
================================
 light.cpp

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "light.h"

/*
================
 Light::Light
================
*/
Light::Light(void)
{

}

/*
================
 Light::~Light
================
*/
Light::~Light(void)
{

}

/*
================
 Light::setDiffuseColor
================
*/
void Light::setDiffuseColor( float red, float green, float blue, float alpha )
{
	diffuseColor_ = D3DXVECTOR4(red, green, blue, alpha);
}

/*
================
 Light::getDiffuseColor
================
*/
D3DXVECTOR4 Light::getDiffuseColor() const
{
	return diffuseColor_;
}

/*
================
 Light::getAmbientColor
================
*/
D3DXVECTOR4 Light::getAmbientColor() const
{
	return ambientColor_;
}

/*
================
 Light::setAmbientColor
================
*/
void Light::setAmbientColor( float red, float green, float blue, float alpha )
{
	ambientColor_ = D3DXVECTOR4(red, green, blue, alpha);
}

void Light::setPosition( float x, float y, float z)
{
	position_ = D3DXVECTOR3(x, y, z);
}

void Light::setLookAt( float x, float y, float z)
{
	lookAt_.x = x;
	lookAt_.y = y;
	lookAt_.z = z;
}

void Light::generateViewMatrix()
{
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&viewMatrix_, &position_, &lookAt_, &up);
}

void Light::generateProjectionMatrix(float screenDepth, float screenNear)
{
	float fov = (float)D3DX_PI * 0.5f;
	float screenAspect = 1.0f;

	D3DXMatrixPerspectiveFovLH(&projectionMatrix_, fov, screenAspect, screenNear, screenDepth);
}

void Light::getViewMatrix( D3DXMATRIX& viewMatrix )
{
	viewMatrix = viewMatrix_;
}

void Light::getProjectionMatrix( D3DXMATRIX& projectionMatrix )
{
	projectionMatrix = projectionMatrix_;
}

D3DXVECTOR3 Light::getPosition() const
{
	return position_;
}

void Light::setDirection( float x, float y, float z )
{
	direction_ = D3DXVECTOR3(x, y, z);
}

D3DXVECTOR3 Light::getDirection() const
{
	return direction_;
}


