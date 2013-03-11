/*
================================
 camera.cpp
 
 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "camera.h"

/*
================
 Camera::Camera
================
*/
Camera::Camera(void)
{

}

/*
================
 Camera::~Camera
================
*/
Camera::~Camera(void)
{

}

/*
================
 Camera::render

 Creates view matrix based on position and rotation.
================
*/
void Camera::render()
{
	D3DXVECTOR3 up;
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;	

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = x_;
	position.y = y_;
	position.z = z_;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// radians
	float pitch = rx_ * 0.0174532925f;
	float yaw = ry_ * 0.0174532925f;
	float roll = rz_ * 0.0174532925f;

	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Create view matrix
	D3DXMatrixLookAtLH(&viewMatrix_, &position, &lookAt, &up);
}

/*
================
 Camera::getViewMatrix
================
*/
void Camera::getViewMatrix( D3DXMATRIX& viewMatrix )
{
	viewMatrix = viewMatrix_;
}
