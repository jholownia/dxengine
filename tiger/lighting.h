/*
================================
 lighting.h

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <D3DX10math.h>
/*
================================
 
 Lighting

 Class to hold lighting information.
 
================================
*/
class Lighting
{
public:
	Lighting(void);
	virtual ~Lighting(void);

	void setAmbientColor(float, float, float, float);
	void setDiffuseColor(float, float, float, float);
	void setDirection(float, float, float);

	D3DXVECTOR4 getAmbientColor() const;
	D3DXVECTOR4 getDiffuseColor() const;
	D3DXVECTOR3 getDirection() const;

private:
	D3DXVECTOR4 ambientColor_;
	D3DXVECTOR4 diffuseColor_;
	D3DXVECTOR3 direction_;
};

