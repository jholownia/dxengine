/*
================================
 shadermanager.cpp
 
 Created on: 07 Jan 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012 - 2013.
================================
*/

#include "shadermanager.h"

/*
================
 ShaderManager::ShaderManager
================
*/
ShaderManager::ShaderManager(void) :
	shader_        (NULL),
	terrainShader_ (NULL)
{

}

/*
================
 haderManager::~ShaderManager
================
*/
ShaderManager::~ShaderManager(void)
{

}

/*
================
 ShaderManager::init

 All shaders are initialized here.
================
*/
bool ShaderManager::init( ID3D10Device* device, HWND hwnd)
{
	bool result;

	shader_ = new Shader;
	result = shader_->init(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize shader", L"Error", MB_OK);
		return false;
	}

	terrainShader_ = new TerrainShader;
	result = terrainShader_->init(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize terrain shader", L"Error", MB_OK);
		return false;
	}

	return true;
}

/*
================
 ShaderManager::shutdown
================
*/
void ShaderManager::shutdown()
{
	if (terrainShader_)
	{
		terrainShader_->shutdown();
		delete terrainShader_;
		terrainShader_ = NULL;
	}
	
	if (shader_)
	{
		shader_->shutdown();
		delete shader_;
		shader_ = NULL;
	}
}

/*
================
 ShaderManager::renderLightShader
================
*/
void ShaderManager::renderLightShader( ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, float wingAngle, Shader::Technique technique )
{
	shader_->render(device, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, ambientColor, diffuseColor, wingAngle, technique);
}

/*
================
 ShaderManager::renderTerrainShader

 This method currently only sets shader parameters due to the way terrain rendering works.
 Draw() function is now called from the quadtree, to only render visible nodes.
================
*/
void ShaderManager::renderTerrainShader( ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 lightDirection, ID3D10ShaderResourceView* texture )
{
	terrainShader_->setShaderParameters(worldMatrix, viewMatrix, projectionMatrix, ambientColor, diffuseColor, lightDirection, texture );
	// terrainShader_->renderShader(device, indexCount);
}

/*
================
 ShaderManager::getLightShader
================
*/
Shader* ShaderManager::getLightShader()
{
	return shader_;
}

/*
================
 ShaderManager::getTerrainShader
================
*/
TerrainShader* ShaderManager::getTerrainShader()
{
	return terrainShader_;
}
