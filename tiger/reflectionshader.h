/*
================================
 seflectionshader.h

 Created on: 20 Mar 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#pragma once

#include <d3d10.h>
#include <D3DX10.h>
#include <fstream>

#include "common.h"


/*
================================
 
 ReflectionShader

 Shader object implementing basic planar reflection.
 
================================
*/
class ReflectionShader : Uncopyable
{
public:
	ReflectionShader(void);
	~ReflectionShader(void);

	bool init(ID3D10Device*, HWND);
	void shutdown();
	void render(ID3D10Device*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D10ShaderResourceView*, ID3D10ShaderResourceView*, D3DXMATRIX);

private:
	ID3D10Effect* effect_;
	ID3D10EffectTechnique* technique_;
	ID3D10InputLayout* layout_;
	ID3D10EffectMatrixVariable* worldMatrixPtr_;
	ID3D10EffectMatrixVariable* viewMatrixPtr_;
	ID3D10EffectMatrixVariable* projectionMatrixPtr_;
	ID3D10EffectShaderResourceVariable* texturePtr_;	
	ID3D10EffectShaderResourceVariable* reflectionTexturePtr_;
	ID3D10EffectMatrixVariable* reflectionMatrixPtr_;

	bool initShader(ID3D10Device*, HWND, WCHAR*);
	void shutdownShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	void setShaderParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D10ShaderResourceView*, ID3D10ShaderResourceView*, D3DXMATRIX);
	void renderShader(ID3D10Device*, int);
};