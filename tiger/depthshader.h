/*
================================
 depthshader.h

 Created on: 21 Mar 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <d3d10.h>
#include <D3DX10.h>
#include <fstream>

#include "common.h"


/*
================================
 
 DepthShader

 Simple shader for rendering depth.
 
================================
*/
class DepthShader : Uncopyable
{
public:
	DepthShader(void);
	~DepthShader(void);

	bool init(ID3D10Device*, HWND);
	void shutdown();
	void render(ID3D10Device*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

private:
	ID3D10Effect* effect_;
	ID3D10EffectTechnique* technique_;
	ID3D10InputLayout* layout_;
	ID3D10EffectMatrixVariable* worldMatrixPtr_;
	ID3D10EffectMatrixVariable* viewMatrixPtr_;
	ID3D10EffectMatrixVariable* projectionMatrixPtr_;


	bool initShader(ID3D10Device*, HWND, WCHAR*);
	void shutdownShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	void setShaderParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void renderShader(ID3D10Device*, int);
};

