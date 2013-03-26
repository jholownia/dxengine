/*
================================
 shadermanager.h
 
 Created on: 07 Jan 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012 - 2013.
================================
*/

#pragma once

#include "d3dmanager.h"
#include "shader.h"
#include "terrainshader.h"
#include "transparentshader.h"
#include "reflectionshader.h"
#include "refractionshader.h"
#include "watershader.h"
#include "shadowshader.h"
#include "depthshader.h"

/*
================================
 
 ShaderManager

  A wrapper for multiple shaders.
 
================================
*/
class ShaderManager : Uncopyable
{
public:
	ShaderManager(void);
	~ShaderManager(void);

	bool init(ID3D10Device*, HWND);
	void shutdown();

	Shader* getLightShader();
	TerrainShader* getTerrainShader();
	TransparentShader* getTransparentShader();
	ReflectionShader* getReflectionShader();
		
	void renderLightShader(ID3D10Device* , int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D10ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, float, Shader::Technique);
	void renderTerrainShader(ID3D10Device* , int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, ID3D10ShaderResourceView* );
	void renderTransparentShader( ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView* texture, float blend);
	void renderReflectionShader( ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView* texture, ID3D10ShaderResourceView* reflectionTexture, D3DXMATRIX reflectionMatrix);
	void renderRefractionShader( ID3D10Device* , int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D10ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4);
	void renderWaterShader( ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXMATRIX reflectionMatrix, ID3D10ShaderResourceView* reflectionTexture, ID3D10ShaderResourceView* refractionTexture, ID3D10ShaderResourceView* normalTexture, float waterTranslation, float reflectRefractScale );
	void renderShadowShader(  ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXMATRIX lightViewMatrix, D3DXMATRIX lightProjectionMatrix, ID3D10ShaderResourceView* texture, ID3D10ShaderResourceView* depthMapTexture, D3DXVECTOR3 lightPosition, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor );
	void renderDepthShader( ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix );

private:
	Shader* shader_;
	TerrainShader* terrainShader_;
	TransparentShader* transparentShader_;
	ReflectionShader* reflectionShader_;
	RefractionShader* refractionShader_;
	WaterShader* waterShader_;
	ShadowShader* shadowShader_;
	DepthShader* depthShader_;
};

