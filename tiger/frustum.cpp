/*
================================
 frustum.cpp
 
 Created on: 22 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "frustum.h"

/*
================
 Frustum::Frustum
================
*/
Frustum::Frustum(void)
{

}

/*
================
 Frustum::~Frustum
================
*/
Frustum::~Frustum(void)
{

}

/*
================
 Frustum::createFrustum
================
*/
void Frustum::createFrustum( float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix )
{
	
	float r;
	D3DXMATRIX matrix;

	// Calculate the minimum z distance in the frustum
	float zMin = -projectionMatrix._43 / projectionMatrix._33;
	r = screenDepth / (screenDepth - zMin);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * zMin;

	// Create the frustum matrix from the view matrix and update projection matrix
	D3DXMatrixMultiply(&matrix, &viewMatrix, &projectionMatrix);

	// Calculate near plane of frustum.
	planes_[0].a = matrix._14 + matrix._13;
	planes_[0].b = matrix._24 + matrix._23;
	planes_[0].c = matrix._34 + matrix._33;
	planes_[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&planes_[0], &planes_[0]);

	// Calculate far plane of frustum.
	planes_[1].a = matrix._14 - matrix._13; 
	planes_[1].b = matrix._24 - matrix._23;
	planes_[1].c = matrix._34 - matrix._33;
	planes_[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&planes_[1], &planes_[1]);

	// Calculate left plane of frustum.
	planes_[2].a = matrix._14 + matrix._11; 
	planes_[2].b = matrix._24 + matrix._21;
	planes_[2].c = matrix._34 + matrix._31;
	planes_[2].d = matrix._44 + matrix._41;
	D3DXPlaneNormalize(&planes_[2], &planes_[2]);

	// Calculate right plane of frustum.
	planes_[3].a = matrix._14 - matrix._11; 
	planes_[3].b = matrix._24 - matrix._21;
	planes_[3].c = matrix._34 - matrix._31;
	planes_[3].d = matrix._44 - matrix._41;
	D3DXPlaneNormalize(&planes_[3], &planes_[3]);

	// Calculate top plane of frustum.
	planes_[4].a = matrix._14 - matrix._12; 
	planes_[4].b = matrix._24 - matrix._22;
	planes_[4].c = matrix._34 - matrix._32;
	planes_[4].d = matrix._44 - matrix._42;
	D3DXPlaneNormalize(&planes_[4], &planes_[4]);

	// Calculate bottom plane of frustum.
	planes_[5].a = matrix._14 + matrix._12;
	planes_[5].b = matrix._24 + matrix._22;
	planes_[5].c = matrix._34 + matrix._32;
	planes_[5].d = matrix._44 + matrix._42;
	D3DXPlaneNormalize(&planes_[5], &planes_[5]);
}

/*
================
 Frustum::checkPoint
================
*/
bool Frustum::checkPoint( float x, float y, float z )
{
	for (int i = 0; i < 6; ++i)
	{
		if (D3DXPlaneDotCoord(&planes_[i], &D3DXVECTOR3(x, y, z)) < 0.0f)
		{
			return false;
		}
	}

	return true;
}

/*
================
 Frustum::checkCube
================
*/
bool Frustum::checkCube( float xCenter, float yCenter, float zCenter, float radius )
{
	for(int i=0; i<6; ++i) 
	{
		if(D3DXPlaneDotCoord(&planes_[i], &D3DXVECTOR3((xCenter - radius), (yCenter - radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&planes_[i], &D3DXVECTOR3((xCenter + radius), (yCenter - radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&planes_[i], &D3DXVECTOR3((xCenter - radius), (yCenter + radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&planes_[i], &D3DXVECTOR3((xCenter + radius), (yCenter + radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&planes_[i], &D3DXVECTOR3((xCenter - radius), (yCenter - radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&planes_[i], &D3DXVECTOR3((xCenter + radius), (yCenter - radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&planes_[i], &D3DXVECTOR3((xCenter - radius), (yCenter + radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&planes_[i], &D3DXVECTOR3((xCenter + radius), (yCenter + radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

/*
================
 Frustum::checkSphere
================
*/
bool Frustum::checkSphere( float x, float y, float z, float radius )
{
	for(int i=0; i<6; ++i) 
	{
		if(D3DXPlaneDotCoord(&planes_[i], &D3DXVECTOR3(x, y, z)) < -radius)
		{
			return false;
		}
	}

	return true;
}