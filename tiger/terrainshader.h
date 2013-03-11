/*
================================
 terrainshader.h
 
 Created on: 10 Jan 2012
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
 
 TerrainShader

 A shader class for rendering terrain.
 
================================
*/
class TerrainShader : Uncopyable
{
public:
	TerrainShader(void);
	~TerrainShader(void);

	bool init(ID3D10Device* device, HWND hwnd);
	void shutdown();

	void setShaderParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, ID3D10ShaderResourceView*);
	void renderShader(ID3D10Device*, int);

private:
	ID3D10Effect* effect_;
	ID3D10EffectTechnique* technique_;
	ID3D10InputLayout* layout_;
	ID3D10SamplerState* samplerState_;
	ID3D10EffectMatrixVariable* worldMatrixPtr_;
	ID3D10EffectMatrixVariable* viewMatrixPtr_;
	ID3D10EffectMatrixVariable* projectionMatrixPtr_;
	ID3D10EffectVectorVariable* ambientColorPtr_;
	ID3D10EffectVectorVariable* diffuseColorPtr_;
	ID3D10EffectVectorVariable* lightDirectionPtr_;
	ID3D10EffectShaderResourceVariable* texturePtr_;
	
	bool initShader(ID3D10Device*, HWND, WCHAR* );
	void shutdownShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR* );	
};

