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
	shader_            (NULL),
	terrainShader_     (NULL),
	transparentShader_ (NULL)
{

}

/*
================
 ShaderManager::~ShaderManager
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

	transparentShader_ = new TransparentShader;
	result = transparentShader_->init(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize transparent shader", L"Error", MB_OK);
		return false;
	}

	reflectionShader_ = new ReflectionShader;
	result = reflectionShader_->init(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize reflection shader", L"Error", MB_OK);
		return false;
	}

	refractionShader_ = new RefractionShader;
	result = refractionShader_->init(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize refraction shader", L"Error", MB_OK);
		return false;
	}

	waterShader_ = new WaterShader;
	result = waterShader_->init(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize water shader", L"Error", MB_OK);
		return false;
	}

	shadowShader_ = new ShadowShader;
	result = shadowShader_->init(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize shadow shader", L"Error", MB_OK);
		return false;
	}

	depthShader_ = new DepthShader;
	result = depthShader_->init(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize depth shader", L"Error", MB_OK);
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
	if (depthShader_)
	{
		depthShader_->shutdown();
		delete depthShader_;
		depthShader_ = NULL;
	}
	
	if (shadowShader_)
	{
		shadowShader_->shutdown();
		delete shadowShader_;
		shadowShader_ = NULL;
	}
	
	if (waterShader_)
	{
		waterShader_->shutdown();
		delete waterShader_;
		waterShader_ = NULL;
	}
	
	if (refractionShader_)
	{
		refractionShader_->shutdown();
		delete refractionShader_;
		refractionShader_ = NULL;
	}

	if (reflectionShader_)
	{
		reflectionShader_->shutdown();
		delete reflectionShader_;
		reflectionShader_ = NULL;
	}
	
	if (transparentShader_)
	{
		transparentShader_->shutdown();
		delete transparentShader_;
		transparentShader_ = NULL;
	}

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
 ShaderManager::renderTransparentShader
================
*/
void ShaderManager::renderTransparentShader( ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView* texture, float blend )
{
	transparentShader_->render(device, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, blend);
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

/*
================
 ShaderManager::getTransparentShader
================
*/
TransparentShader* ShaderManager::getTransparentShader()
{
	return transparentShader_;
}

void ShaderManager::renderReflectionShader( ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView* texture, ID3D10ShaderResourceView* reflectionTexture, D3DXMATRIX reflectionMatrix )
{
	reflectionShader_->render(device, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, reflectionTexture, reflectionMatrix);
}

ReflectionShader* ShaderManager::getReflectionShader()
{
	return reflectionShader_;
}

void ShaderManager::renderRefractionShader( ID3D10Device* device , int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR4 clipPlane )
{
	refractionShader_->render(device, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, ambientColor, diffuseColor, clipPlane);
}

void ShaderManager::renderWaterShader( ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXMATRIX reflectionMatrix, ID3D10ShaderResourceView* reflectionTexture, ID3D10ShaderResourceView* refractionTexture, ID3D10ShaderResourceView* normalTexture, float waterTranslation, float reflectRefractScale  )
{
	waterShader_->render(device, indexCount, worldMatrix, viewMatrix, projectionMatrix, reflectionMatrix, reflectionTexture, refractionTexture, normalTexture, waterTranslation, reflectRefractScale  );
}

void ShaderManager::renderShadowShader( ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXMATRIX lightViewMatrix, D3DXMATRIX lightProjectionMatrix, ID3D10ShaderResourceView* texture, ID3D10ShaderResourceView* depthMapTexture, D3DXVECTOR3 lightPosition, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor )
{
	shadowShader_->render( device, indexCount, worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix,  texture, depthMapTexture, lightPosition, ambientColor, diffuseColor);
}

void ShaderManager::renderDepthShader( ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix )
{
	depthShader_->render(device, indexCount, worldMatrix, viewMatrix, projectionMatrix);
}

