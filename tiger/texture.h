/*
================================
 texture.h

 Created on: 12 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <d3d10.h>
#include <D3DX10.h>
/*
================================
 
 Texture

 A simple wrapper for texture.
 
================================
*/
class Texture
{
public:
	Texture(void);
	~Texture(void);

	bool init(ID3D10Device* device, WCHAR* filename);
	void shutdown();

	ID3D10ShaderResourceView* getTexture();

private:
	ID3D10ShaderResourceView* texture_;
};

