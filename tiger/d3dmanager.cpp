/*
================================
 d3dmanager.h

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "d3dmanager.h"


/*
================
 D3DManager::D3DManager
================
*/
D3DManager::D3DManager(void) :
	swapChain_                (NULL),
	device_                   (NULL),
	renderTargetView_         (NULL),
	depthStencilBuffer_       (NULL),
	depthStencilState_        (NULL),
	depthStencilView_         (NULL),
	rasterizerState_          (NULL),
	depthDisableStencilState_ (NULL)
{

}

/*
================
 D3DManager::~D3DManager
================
*/
D3DManager::~D3DManager(void)
{

}

/*
================
 D3DManager::init

 Sets up device, swap chain, back buffer, depth buffer and matrices.
================
*/
// FIXME: split into multiple methods for readibility
bool D3DManager::init( int screenWidth, int screenHeight, const bool vsync, HWND hwnd, const bool fullscreen, const float screenDepth, const float screenNear )
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes;	
	unsigned int numerator;
	unsigned int denominator;
	unsigned int stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D10Texture2D* backBufferPtr;
	D3D10_TEXTURE2D_DESC depthBufferDesc;
	D3D10_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;	
	float fov;
	float aspectRatio;
	D3D10_RASTERIZER_DESC rasterizerDesc;

	vsyncEnabled_ = vsync;

	// Get refresh rates and find a correct one to use
	// ----------------------------------------------------------------------

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
	{
		return false;
	}
	
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Find correct display mode
	for (unsigned int i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == (unsigned int) screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int) screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get GPU name and memory
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store memory in megabytes
	gpuMemory_ = (int) (adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Get GPU name
	error = wcstombs_s(&stringLength, gpuDescription_, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Clean up
	delete[] displayModeList;
	displayModeList = NULL;

	adapterOutput->Release();
	adapterOutput = NULL;

	adapter->Release();
	adapter = NULL;

	factory->Release();
	factory = 0;

	//------------------------------------------------------------------------------
	
	// Start actual directx initialization
	// ------------------------------------------------------------------------------------

	// Swap chain
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (vsyncEnabled_)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;

	// no multisampling - 1, 0
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	

	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Finally create the device
	result = D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &swapChainDesc, &swapChain_, &device_);
	

	// Back buffer
	result = swapChain_->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*) &backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	result = device_->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView_);
	if (FAILED(result))
	{
		return false;
	}

	// Clean up
	backBufferPtr->Release();
	backBufferPtr = NULL;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;  // 1
	depthBufferDesc.SampleDesc.Quality = 0;  // 0
	depthBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = device_->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer_);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	// Create depth stencil state
	result = device_->CreateDepthStencilState(&depthStencilDesc, &depthStencilState_);
	if (FAILED(result))
	{
		return false;
	}

	device_->OMSetDepthStencilState(depthStencilState_, 1);

	// Initialize depth stencil view
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create depth stencil view
	result = device_->CreateDepthStencilView(depthStencilBuffer_, &depthStencilViewDesc, &depthStencilView_);

	// Bind render target view
	device_->OMSetRenderTargets(1, &renderTargetView_, depthStencilView_);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D10_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D10_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;  // false
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	result = device_->CreateRasterizerState(&rasterizerDesc, &rasterizerState_);
	if(FAILED(result))
	{
		return false;
	}

	device_->RSSetState(rasterizerState_);

	// Setup the viewport
	viewport_.Width = screenWidth;
	viewport_.Height = screenHeight;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;

	// Create the viewport.
	device_->RSSetViewports(1, &viewport_);

	// Setup projection matrix
	fov = (float) D3DX_PI / 4.0f;
	aspectRatio = (float) screenWidth / (float) screenHeight;

	D3DXMatrixPerspectiveFovLH(&projectionMatrix_, fov, aspectRatio, screenNear, screenDepth);

	// Initialize world matrix
	D3DXMatrixIdentity(&worldMatrix_);

	// Create camera here

	// Create an orthographic projection matrix for 2D rendering
	D3DXMatrixOrthoLH(&orthoMatrix_, (float) screenWidth, (float) screenHeight, screenNear, screenDepth);

	// Depth stencil stuff
	// ----------------------------------------------------------------------------------------------
	D3D10_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Create second depth stencil state which turns off the Z buffer for 2D rendering
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	// Create the state using the device
	result = device_->CreateDepthStencilState(&depthDisabledStencilDesc, &depthDisableStencilState_);
	if (FAILED(result))
	{
		return false;
	}
	
	return true;
}

/*
================
 D3DManager::shutdown
================
*/
void D3DManager::shutdown()
{
	if (swapChain_)
	{
		swapChain_->SetFullscreenState(false, NULL);
	}

	if (depthDisableStencilState_)
	{
		depthDisableStencilState_->Release();
		depthDisableStencilState_ = NULL;
	}

	if (rasterizerState_)
	{
		rasterizerState_->Release();
		rasterizerState_ = NULL;
	}

	if (depthStencilView_)
	{
		depthStencilView_->Release();
		depthStencilView_ = NULL;
	}

	if (depthStencilState_)
	{
		depthStencilState_->Release();
		depthStencilState_ = NULL;
	}

	if (depthStencilBuffer_)
	{
		depthStencilBuffer_->Release();
		depthStencilBuffer_ = NULL;
	}

	if (renderTargetView_)
	{
		renderTargetView_->Release();
		renderTargetView_ = NULL;
	}

	if (swapChain_)
	{
		swapChain_->Release();
		swapChain_ = NULL;
	}

	if (device_)
	{
		device_->Release();
		device_ = NULL;
	}
}

/*
================
 D3DManager::beginScene

 Clears render target and back buffer to prepare for rendering.
================
*/
void D3DManager::beginScene( float red, float green, float blue, float alpha )
{
	float color[4];
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer
	device_->ClearRenderTargetView(renderTargetView_, color);

	// Clear the depth buffer
	device_->ClearDepthStencilView(depthStencilView_, D3D10_CLEAR_DEPTH, 1.0f, 0);
}

/*
================
 D3DManager::endScene

 Shows the rendered image.
================
*/
void D3DManager::endScene()
{
	// Present the back buffer to the screen when the rendering is complete
	if (vsyncEnabled_)
	{
		swapChain_->Present(1,0);
	}
	else
	{
		swapChain_->Present(0,0);  // present asap
	}
}

/*
================
 D3DManager::getDevice
================
*/
ID3D10Device* D3DManager::getDevice()
{
	return device_;
}

/*
================
 D3DManager::getProjectionMatrix
================
*/
void D3DManager::getProjectionMatrix( D3DXMATRIX& projectionMatrix )
{
	projectionMatrix = projectionMatrix_;
}

/*
================
 D3DManager::getWorldMatrix
================
*/
void D3DManager::getWorldMatrix( D3DXMATRIX& worldMatrix )
{
	worldMatrix = worldMatrix_;
}

/*
================
 D3DManager::getOrthoMatrix
================
*/
void D3DManager::getOrthoMatrix( D3DXMATRIX& orthoMatrix )
{
	orthoMatrix = orthoMatrix_;
}

/*
================
 D3DManager::getGpuInfo
================
*/
void D3DManager::getGpuInfo( char* gpuName, int& memory )
{
	strcpy_s(gpuName, 128, gpuDescription_);
	memory = gpuMemory_;
}

/*
================
 D3DManager::turnZBufferOn
================
*/
void D3DManager::turnZBufferOn()
{
	device_->OMSetDepthStencilState(depthStencilState_, 1);
}

/*
================
 D3DManager::turnZBufferOff
================
*/
void D3DManager::turnZBufferOff()
{
	device_->OMSetDepthStencilState(depthDisableStencilState_, 1);
}

/*
================
 D3DManager::getDepthStencilView
================
*/
ID3D10DepthStencilView* D3DManager::getDepthStencilView()
{
	return depthStencilView_;
}

/*
================
 D3DManager::setBackBufferRenderTarget
================
*/
void D3DManager::setBackBufferRenderTarget()
{
	device_->OMSetRenderTargets(1, &renderTargetView_, depthStencilView_);
}

void D3DManager::resetViewport()
{
	device_->RSSetViewports(1, &viewport_);
}
