/*
================================
 rendertexture.cpp

 Created on: 20 Mar 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#include "rendertexture.h"


/*
================
 RenderTexture::RenderTexture
================
*/
RenderTexture::RenderTexture(void) :
	renderTargetTexture_ (NULL),
	renderTargetView_    (NULL),
	shaderResourceView_  (NULL)
{

}

/*
================
 RenderTexture::~RenderTexture
================
*/
RenderTexture::~RenderTexture(void)
{

}

/*
================
 RenderTexture::init

 Creates a texture and sets it up as render target view so that it can be rendered onto.
================
*/
bool RenderTexture::init( ID3D10Device* device, int textureWidth, int textureHeight)
{
	D3D10_TEXTURE2D_DESC textureDesc;
	
	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.	
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;	
	textureDesc.Usage = D3D10_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target texture.
	HRESULT result = device->CreateTexture2D(&textureDesc, NULL, &renderTargetTexture_);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the render target view.
	D3D10_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = device->CreateRenderTargetView(renderTargetTexture_, &renderTargetViewDesc, &renderTargetView_);
	if(FAILED(result))
	{
		return false;
	}
	
	// Setup the description of the shader resource view.
	D3D10_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = device->CreateShaderResourceView(renderTargetTexture_, &shaderResourceViewDesc, &shaderResourceView_);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}
/*
================
 RenderTexture::shutdown
================
*/
void RenderTexture::shutdown()
{
	if (shaderResourceView_)
	{
		shaderResourceView_->Release();
		shaderResourceView_ = NULL;
	}

	if (renderTargetView_)
	{
		renderTargetView_->Release();
		renderTargetView_ = NULL;
	}

	if (renderTargetTexture_)
	{
		renderTargetTexture_->Release();
		renderTargetTexture_ = NULL;
	}
}

/*
================
 RenderTexture::RenderTexture

 Sets rendering location to renderTargetView_
================
*/
void RenderTexture::setRenderTarget( ID3D10Device* device, ID3D10DepthStencilView* depthStencilView)
{
	device->OMSetRenderTargets(1, &renderTargetView_, depthStencilView);
}
/*
================
 RenderTexture::RenderTexture

 Clears the texture before rendering (similar to clearing back buffer).
================
*/
void RenderTexture::clearRenderTarget( ID3D10Device* device, ID3D10DepthStencilView* depthStencilView, float r, float g, float b, float a)
{
	float color[4] = {r, g, b, a};

	device->ClearRenderTargetView(renderTargetView_, color);
	device->ClearDepthStencilView(depthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);
}
/*
================
 RenderTexture::getShaderResourceView
================
*/
ID3D10ShaderResourceView* RenderTexture::getShaderResourceView()
{
	return shaderResourceView_;
}
