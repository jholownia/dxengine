/*
================================
 rendertexture.h

 Created on: 20 Mar 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#pragma once

#include <d3d10.h>

#include "common.h"

/*
================================
 
 RenderTexture

 This class allows rendering to a texture instead of a back buffer.

================================
*/

class RenderTexture : Uncopyable
{
public:
	RenderTexture(void);
	~RenderTexture(void);

	bool init(ID3D10Device*, int, int);
	void shutdown();

	void setRenderTarget(ID3D10Device*, ID3D10DepthStencilView*);
	void clearRenderTarget(ID3D10Device*, ID3D10DepthStencilView*, float, float, float, float);
	ID3D10ShaderResourceView* getShaderResourceView();

private:
	ID3D10Texture2D* renderTargetTexture_;
	ID3D10RenderTargetView* renderTargetView_;
	ID3D10ShaderResourceView* shaderResourceView_;

};

