/*
================================
 shader.h
 
 Created on: 09 Dec 2012
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
 
 Shader

 The default shader class. The corresponding light.fx shader
 is used for rendering moveable objects. This class allows choosing which
 technique within the shader is used for rendering.
 
================================
*/
class Shader : Uncopyable
{
public:
	Shader(void);	
	~Shader(void);
		
	enum Technique
	{
		DEFAULT,    // default technique
		LWING,      // left wing (mesh deformation)
		RWING       // right wing
	};

	bool init(ID3D10Device* device, HWND hwnd);
	void shutdown();
	void render(ID3D10Device*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D10ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, float, Technique);
		
private:
	ID3D10Effect* effect_;
	ID3D10EffectTechnique* technique_;
	ID3D10EffectTechnique* techniqueRightWing_;
	ID3D10EffectTechnique* techniqueLeftWing_;
	ID3D10InputLayout* layout_;
	ID3D10EffectMatrixVariable* worldMatrixPtr_;
	ID3D10EffectMatrixVariable* viewMatrixPtr_;
	ID3D10EffectMatrixVariable* projectionMatrixPtr_;
	ID3D10EffectShaderResourceVariable* texturePrt_;
	ID3D10EffectVectorVariable* lightDirectionPtr_;
	ID3D10EffectVectorVariable* ambientColorPtr_;
	ID3D10EffectVectorVariable* diffuseColorPtr_;
	ID3D10EffectScalarVariable* wingAnglePtr_;
	
	bool initShader(ID3D10Device*, HWND, WCHAR* );
	void shutdownShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR* );
	void setShaderParameters(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D10ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, float);
	void renderShader(ID3D10Device*, int, Technique);
};

