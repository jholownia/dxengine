/*
================================
 frustum.h
 
 Created on: 22 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <D3DX10math.h>
#include <array>

#include "common.h"


/*
================================
 
 Frustum

 This class represents a frustum consisting of six planes
 which is used fo culling.
 
================================
*/
class Frustum : Uncopyable
{
public:
	Frustum(void);
	~Frustum(void);

	void createFrustum(float, D3DXMATRIX, D3DXMATRIX);

	bool checkPoint(float, float, float);
	bool checkCube(float, float, float, float);
	bool checkSphere(float, float, float, float);	

private:
	std::array<D3DXPLANE, 6> planes_;
};

