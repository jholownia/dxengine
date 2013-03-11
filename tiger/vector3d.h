/*
================================
 vector3d.h

 Created on: 09 Jan 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#pragma once


/*
================================
 
 Vector3D

 A vector class like the Directx one, for convenience.
 
================================
*/
struct Vector3D
{
	float x;
	float y;
	float z;

	Vector3D () { };
	Vector3D (float x, float y, float z) : x(x), y(y), z(z) { };

	Vector3D operator+(Vector3D const& other)
	{
		Vector3D result;
		
		result.x = x + other.x;
		result.y = y + other.y;
		result.z = z + other.z;

		return result;
	}

	Vector3D operator*(float scalar)
	{
		Vector3D result;

		result.x = x * scalar;
		result.y = y * scalar;
		result.z = z * scalar;

		return result;
	}
};