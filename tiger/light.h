/*
================================
 light.h

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <D3DX10math.h>
/*
================================
 
 Light

 Class to hold light information.
 
================================
*/
class Light
{
public:
	Light(void);
	virtual ~Light(void);

	void setAmbientColor(float, float, float, float);
	void setDiffuseColor(float, float, float, float);		
	void setPosition(float, float, float);
	void setLookAt(float, float, float);
		
	void setDirection(float, float, float);

	D3DXVECTOR4 getAmbientColor() const;
	D3DXVECTOR4 getDiffuseColor() const;
	D3DXVECTOR3 getPosition() const;

	D3DXVECTOR3 getDirection() const;

	void generateViewMatrix();
	void generateProjectionMatrix(float, float);

	void getViewMatrix(D3DXMATRIX&);
	void getProjectionMatrix(D3DXMATRIX&);

private:
	D3DXVECTOR4 ambientColor_;
	D3DXVECTOR4 diffuseColor_;
	D3DXVECTOR3 position_;
	D3DXVECTOR3 lookAt_;
	D3DXMATRIX viewMatrix_;
	D3DXMATRIX projectionMatrix_;

	D3DXVECTOR3 direction_;
};

