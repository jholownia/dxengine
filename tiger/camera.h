/*
================================
 camera.h

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include "moveable.h"
#include "common.h"

/*
================================
 
 Camera

 A simple camera class. Holds a view matrix.
 
================================
*/
class Camera : public Moveable, Uncopyable
{
public:
	Camera(void);
	~Camera(void);	

	void render();
	void getViewMatrix(D3DXMATRIX& viewMatrix);

protected:	
	D3DXMATRIX viewMatrix_;
};

