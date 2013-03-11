/*
================================
 d3dmanager.h

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "dxgi.lib")

#include <string>
#include <d3d10.h>
#include <D3DX10.h>

#include "common.h"

/*
================================
 
 D3DManager

 A wrapper class for Direct3D initialization.
 Creates the device sets up swap chain and the back buffer.
 
================================
*/
class D3DManager : Uncopyable
{
public:
	D3DManager(void);	
	~D3DManager(void);

	bool init( int screenWidth, int screenHeight, const bool vsync, HWND hwnd, const bool fullscreen, const float screenDepth, const float screenNear );
	void shutdown();
	
	void beginScene( float r, float g, float b, float a );
	void endScene();

	ID3D10Device* getDevice();

	void getProjectionMatrix(D3DXMATRIX&);
	void getWorldMatrix(D3DXMATRIX&);
	void getOrthoMatrix(D3DXMATRIX&);

	void getGpuInfo(char*, int&); // FIXME: change to string

	void turnZBufferOn();
	void turnZBufferOff();

private:
	bool vsyncEnabled_;
	int gpuMemory_;
	char gpuDescription_[128];    // FIXME
	IDXGISwapChain* swapChain_;
	ID3D10Device* device_;
	ID3D10RenderTargetView* renderTargetView_;
	ID3D10Texture2D* depthStencilBuffer_;
	ID3D10DepthStencilState* depthStencilState_;
	ID3D10DepthStencilView* depthStencilView_;
	ID3D10RasterizerState* rasterizerState_;
	D3DXMATRIX projectionMatrix_;
	D3DXMATRIX worldMatrix_;
	D3DXMATRIX orthoMatrix_;
	ID3D10DepthStencilState* depthDisableStencilState_;
};

