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


/*
================================
 
 ShaderManager

  A wrapper for multiple shaders. Barely used at the moment.
 
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
		
	void renderLightShader(ID3D10Device* , int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D10ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, float, Shader::Technique);
	void renderTerrainShader(ID3D10Device* , int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, ID3D10ShaderResourceView* );
	
private:
	Shader* shader_;
	TerrainShader* terrainShader_;
};

