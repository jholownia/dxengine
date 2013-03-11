/*
================================
 texture.cpp

 Created on: 12 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "texture.h"

/*
================
 Texture::Texture
================
*/
Texture::Texture(void) :
	texture_ (NULL)
{

}

/*
================
 Texture::~Texture
================
*/
Texture::~Texture(void)
{

}

/*
================
 Texture::init
================
*/
bool Texture::init( ID3D10Device* device, WCHAR* filename )
{
	HRESULT result;
	
	result = D3DX10CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &texture_, NULL);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

/*
================
 Texture::shutdown
================
*/
void Texture::shutdown()
{
	if (texture_)
	{

		texture_->Release();
		texture_ = NULL;
	}
}

/*
================
 Texture::getTexture
================
*/
ID3D10ShaderResourceView* Texture::getTexture()
{
	return texture_;
}
