/*
================================
 terrainshader.cpp
 
 Created on: 10 Jan 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#include "terrainshader.h"

/*
================
 TerrainShader::TerrainShader
================
*/
TerrainShader::TerrainShader(void) :
	effect_              (NULL),
	technique_           (NULL),
	layout_              (NULL),
	samplerState_        (NULL),
	worldMatrixPtr_      (NULL),
	viewMatrixPtr_       (NULL),
	projectionMatrixPtr_ (NULL),
	ambientColorPtr_     (NULL),
	diffuseColorPtr_     (NULL),
	lightDirectionPtr_   (NULL),
	texturePtr_          (NULL)
{

}

/*
================
 TerrainShader::~TerrainShader
================
*/
TerrainShader::~TerrainShader(void)
{

}

/*
================
 TerrainShader::init
================
*/
bool TerrainShader::init( ID3D10Device* device, HWND hwnd )
{
	return initShader(device, hwnd, L"terrain.fx");
}

/*
================
 TerrainShader::shutdown
================
*/
void TerrainShader::shutdown()
{
	shutdownShader();
}

/*
================
 TerrainShader::initShader
================
*/
bool TerrainShader::initShader( ID3D10Device* device, HWND hwnd, WCHAR* filename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	D3D10_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D10_PASS_DESC passDesc;

	D3D10_SAMPLER_DESC samplerDesc;
	D3D10_BUFFER_DESC matrixBufferDesc;
	D3D10_BUFFER_DESC lightBufferDesc;
	ID3D10Blob* vertexShaderBuffer = NULL;
	ID3D10Blob* pixelShaderBuffer = NULL;

	// Initialize the error message.
	errorMessage = 0;
	
	// Load the shader in from the file.
	result = D3DX10CreateEffectFromFile(filename, NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, device, NULL, NULL, &effect_, &errorMessage, NULL);

	if(FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if(errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, filename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, filename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	technique_ = effect_->GetTechniqueByName("TerrainTechnique");
	

	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType structure in the Object3D class and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Get the description of the first pass described in the shader technique.
	technique_->GetPassByIndex(0)->GetDesc(&passDesc);

	// Create the input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &layout_);
	if(FAILED(result))
	{
		return false;
	}

	// -----------------------------------

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &samplerState_);
	if(FAILED(result))
	{
		return false;
	}
	
	// Get pointers to the three matrices inside the shader so we can update them from this class.
	worldMatrixPtr_ = effect_->GetVariableByName("worldMatrix")->AsMatrix();
	viewMatrixPtr_ = effect_->GetVariableByName("viewMatrix")->AsMatrix();
	projectionMatrixPtr_ = effect_->GetVariableByName("projectionMatrix")->AsMatrix();

	// Pointer to the texture resource inside the shader
	texturePtr_ = effect_->GetVariableByName("shaderTexture")->AsShaderResource();

	// Light direction, ambient and diffuse color	
	ambientColorPtr_ = effect_->GetVariableByName("ambientColor")->AsVector();
	diffuseColorPtr_ = effect_->GetVariableByName("diffuseColor")->AsVector();
	lightDirectionPtr_ = effect_->GetVariableByName("lightDirection")->AsVector();

	return true;	
}

/*
================
 TerrainShader::shutdownShader
================
*/
void TerrainShader::shutdownShader()
{
	// FIXME: zero pointers

	if (samplerState_)
	{
		samplerState_->Release();
		samplerState_ = NULL;
	}	
	
	if (layout_)
	{
		layout_->Release();
		layout_ = NULL;
	}

	if(effect_)
	{
		effect_->Release();
		effect_ = NULL;
	}
		
	technique_ = NULL;	
}

/*
================
 TerrainShader::outputShaderErrorMessage
================
*/
void TerrainShader::outputShaderErrorMessage( ID3D10Blob* errorMessage, HWND hwnd, WCHAR* filename )
{
	char* compileErrors;
	unsigned long bufferSize;
	std::ofstream ofs;

	compileErrors = (char *)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();

	ofs.open("shader_errors.txt");

	for (unsigned int i = 0; i < bufferSize; ++i)
	{
		ofs << compileErrors[i];
	}

	ofs.close();

	errorMessage->Release();
	errorMessage = NULL;

	MessageBox(hwnd, L"Error compiling shader. Check shader_errors.txt for details.", filename, MB_OK);
}

/*
================
 TerrainShader::setShaderParameters
================
*/
void TerrainShader::setShaderParameters( D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 lightDirection, ID3D10ShaderResourceView* texture )
{
	// Set the world matrix variable inside the shader.
	worldMatrixPtr_->SetMatrix((float*)&worldMatrix);

	// Set the view matrix variable inside the shader.
	viewMatrixPtr_->SetMatrix((float*)&viewMatrix);

	// Set the projection matrix variable inside the shader.
	projectionMatrixPtr_->SetMatrix((float*)&projectionMatrix);

	// Bind the texture
	texturePtr_->SetResource(texture);

	// Light	
	ambientColorPtr_->SetFloatVector((float *) &ambientColor);
	diffuseColorPtr_->SetFloatVector((float *) &diffuseColor);
	lightDirectionPtr_->SetFloatVector((float *) &lightDirection);		
}

/*
================
 TerrainShader::renderShader
================
*/
void TerrainShader::renderShader( ID3D10Device* device, int indexCount )
{	
	D3D10_TECHNIQUE_DESC techniqueDesc;

	// Set the input layout.
	device->IASetInputLayout(layout_);

	// Get the description structure of the technique from inside the shader so it can be used for rendering.
	technique_->GetDesc(&techniqueDesc);

	// Go through each pass in the technique (should be just one currently) and render the triangles.
	for(unsigned int i = 0; i < techniqueDesc.Passes; ++i)
	{
		technique_->GetPassByIndex(i)->Apply(0);
		device->DrawIndexed(indexCount, 0, 0);
	}

	return;
}
