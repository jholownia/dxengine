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
	
	/*
	// Calculate the rotation in radians.
	float radians = ry_ * 0.0174532925f;

	// Setup where the camera is looking.
	lookAt.x = sinf(radians) + x_;
	lookAt.y = y_;
	lookAt.z = cosf(radians) + z_;

	// Create the view matrix from the three vectors.
	D3DXMatrixLookAtLH(&viewMatrix_, &position, &lookAt, &up);
	*/
		
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

/*
================
 Camera::renderReflection

 This method builds a reflection view matrix. It takes the height of the object to be reflected as a parameter
 and uses it to invert the y position of the reflection.
================
*/
void Camera::renderReflection(float height)
{
	D3DXVECTOR3 up;
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;	

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup camera position with inverted Y
	position.x = x_;
	position.y = -y_ + (height * 2.0f);
	position.z = z_;

	
	// Calculate rotation
	float radians = ry_ * 0.0174532925f;

	// Where the camera is looking
	lookAt.x = sinf(radians) + x_;
	lookAt.y = position.y;
	lookAt.z = cosf(radians) + z_;


	// --







	// --

	// Create reflection matrix
	D3DXMatrixLookAtLH(&reflectionViewMatrix_, &position, &lookAt, &up);	
}

/*
================
 Camera::getReflectionViewMatrix
================
*/
D3DXMATRIX Camera::getReflectionViewMatrix()
{
	return reflectionViewMatrix_;
}
