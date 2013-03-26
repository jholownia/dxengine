/*
================================
 watershader.cpp

 Created on: 21 Mar 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#include "WaterShader.h"

/*
================
 WaterShader::WaterShader
================
*/
WaterShader::WaterShader(void) :
	effect_              (NULL),
	technique_           (NULL),
	layout_              (NULL),
	worldMatrixPtr_      (NULL),
	viewMatrixPtr_       (NULL),
	projectionMatrixPtr_ (NULL),
	reflectionMatrixPtr_ (NULL),
	reflectionTexturePtr_(NULL),
	refractionTexturePtr_(NULL),
	normalTexturePtr_    (NULL),
	translationPtr_      (NULL),
	reflectRefractScalePtr_ (NULL)
	
{

}

/*
================
 WaterShader::~WaterShader
================
*/
WaterShader::~WaterShader(void)
{

}
/*
================
 WaterShader::init
================
*/
bool WaterShader::init( ID3D10Device* device, HWND hwnd)
{
	return initShader(device, hwnd, L"water.fx");
}
/*
================
 WaterShader::shutdown
================
*/
void WaterShader::shutdown()
{
	shutdownShader();
}
/*
================
 WaterShader::render
================
*/
void WaterShader::render( ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXMATRIX reflectionMatrix, ID3D10ShaderResourceView* reflectionTexture, ID3D10ShaderResourceView* refractionTexture, ID3D10ShaderResourceView* normalTexture, float waterTranslation, float reflectRefractScale )
{
	setShaderParameters(worldMatrix, viewMatrix, projectionMatrix, reflectionMatrix, reflectionTexture, refractionTexture, normalTexture, waterTranslation, reflectRefractScale);

	renderShader(device, indexCount);
}
/*
================
 WaterShader::initShader
================
*/
bool WaterShader::initShader( ID3D10Device* device, HWND hwnd, WCHAR* filename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	D3D10_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D10_PASS_DESC passDesc;


	// Initialize the error message.
	errorMessage = 0;

	// Load the shader in from the file.
	result = D3DX10CreateEffectFromFile(filename, NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 
		device, NULL, NULL, &effect_, &errorMessage, NULL);
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

	// Get a pointer to the technique inside the shader.
	technique_ = effect_->GetTechniqueByName("WaterTechnique");
	if(!technique_)
	{
		return false;
	}

	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
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

	// Get pointers to the three matrices inside the shader so we can update them from this class.
	worldMatrixPtr_ = effect_->GetVariableByName("worldMatrix")->AsMatrix();
	viewMatrixPtr_ = effect_->GetVariableByName("viewMatrix")->AsMatrix();
	projectionMatrixPtr_ = effect_->GetVariableByName("projectionMatrix")->AsMatrix();

	// Get a pointer to the reflection matrix resource inside the shader.
	reflectionMatrixPtr_ = effect_->GetVariableByName("reflectionMatrix")->AsMatrix();

	// Get a pointer to the reflection texture resource inside the shader.
	reflectionTexturePtr_ = effect_->GetVariableByName("reflectionTexture")->AsShaderResource();

	// Get a pointer to the refraction texture resource inside the shader.
	refractionTexturePtr_ = effect_->GetVariableByName("refractionTexture")->AsShaderResource();

	// Get a pointer to the normal map texture resource inside the shader.
	normalTexturePtr_ = effect_->GetVariableByName("normalTexture")->AsShaderResource();

	// Get a pointer to the water translation variable inside the shader.
	translationPtr_ = effect_->GetVariableByName("waterTranslation")->AsScalar();

	// Get a pointer to the reflection refraction scale variable inside the shader.
	reflectRefractScalePtr_ = effect_->GetVariableByName("reflectRefractScale")->AsScalar();

	return true;
}
/*
================
 WaterShader::shutdownShader
================
*/
void WaterShader::shutdownShader()
{
	reflectionTexturePtr_ = NULL;
	reflectionMatrixPtr_ = NULL;
	refractionTexturePtr_ = NULL;
	normalTexturePtr_ = NULL;
	worldMatrixPtr_ = NULL;
	projectionMatrixPtr_ = NULL;

	if (layout_)
	{
		layout_->Release();
		layout_ = NULL;
	}

	technique_ = NULL;

	if(effect_)
	{
		effect_->Release();
	}
}
/*
================
 WaterShader::outputShaderErrorMessage
================
*/
void WaterShader::outputShaderErrorMessage( ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream ofs;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	ofs.open("shader_errors.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		ofs << compileErrors[i];
	}

	// Close the file.
	ofs.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader_errors.txt for message.", shaderFilename, MB_OK);

	return;
}

/*
================
 WaterShader::setShaderParameters
================
*/
void WaterShader::setShaderParameters( D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXMATRIX reflectionMatrix, ID3D10ShaderResourceView* reflectionTexture, ID3D10ShaderResourceView* refractionTexture, ID3D10ShaderResourceView* normalTexture, float waterTranslation, float reflectRefractScale)
{
	// Set the world matrix variable inside the shader.
	worldMatrixPtr_->SetMatrix((float*)&worldMatrix);

	// Set the view matrix variable inside the shader.
	viewMatrixPtr_->SetMatrix((float*)&viewMatrix);

	// Set the projection matrix variable inside the shader.
	projectionMatrixPtr_->SetMatrix((float*)&projectionMatrix);
	
	// Bind the reflection texture
	reflectionTexturePtr_->SetResource(reflectionTexture);

	// Set reflection matrix
	reflectionMatrixPtr_->SetMatrix((float*)&reflectionMatrix);

	// Refraction texture
	refractionTexturePtr_->SetResource(refractionTexture);

	normalTexturePtr_->SetResource(normalTexture);
	translationPtr_->SetFloat(waterTranslation);
	reflectRefractScalePtr_->SetFloat(reflectRefractScale);
}

/*
================
 WaterShader::renderShader
================
*/
void WaterShader::renderShader( ID3D10Device* device, int indexCount)
{
	D3D10_TECHNIQUE_DESC techniqueDesc;

	// Set the input layout
	device->IASetInputLayout(layout_);

	// Get the description structure of the technique from inside the shader to be used fo rendering
	technique_->GetDesc(&techniqueDesc);

	// Render
	for (unsigned int i = 0; i < techniqueDesc.Passes; ++i)
	{
		technique_->GetPassByIndex(i)->Apply(0);
		device->DrawIndexed(indexCount, 0, 0);
	}
}
