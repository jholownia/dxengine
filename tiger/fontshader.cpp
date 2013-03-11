/*
================================
 fontshader.cpp

 Created on: 14 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "fontshader.h"

/*
================
 FontShader::FontShader
================
*/
FontShader::FontShader(void) :
	effect_              (NULL),
	technique_           (NULL),
	layout_              (NULL),
	worldMatrixPtr_      (NULL),
	viewMatrixPtr_       (NULL),
	projectionMatrixPtr_ (NULL),
	texturePtr_          (NULL)
{

}

/*
================
 FontShader::~FontShader
================
*/
FontShader::~FontShader(void)
{

}

/*
================
 FontShader::init
================
*/
bool FontShader::init( ID3D10Device* device, HWND hwnd)
{
	return initShader(device, hwnd, L"font.fx");
}

/*
================
 FontShader::shutdown
================
*/
void FontShader::shutdown()
{
	shutdownShader();
}

/*
================
 FontShader::render
================
*/
void FontShader::render( ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView* texture, D3DXVECTOR4 pixelColor)
{
	setShaderParameters(worldMatrix, viewMatrix, projectionMatrix, texture, pixelColor);
	renderShader(device, indexCount);
}

/*
================
 FontShader::initShader
================
*/
bool FontShader::initShader( ID3D10Device* device, HWND hwnd, WCHAR* filename)
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
	technique_ = effect_->GetTechniqueByName("FontTechnique");
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
	result = device->CreateInputLayout(polygonLayout, numElements, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, 
		&layout_);
	if(FAILED(result))
	{
		return false;
	}

	// Get pointers to the three matrices inside the shader so we can update them from this class.
	worldMatrixPtr_ = effect_->GetVariableByName("worldMatrix")->AsMatrix();
	viewMatrixPtr_ = effect_->GetVariableByName("viewMatrix")->AsMatrix();
	projectionMatrixPtr_ = effect_->GetVariableByName("projectionMatrix")->AsMatrix();

	// Get pointer to the texture resource inside the shader.
	texturePtr_ = effect_->GetVariableByName("shaderTexture")->AsShaderResource();

	// Get pointer to the pixel color variable inside the shader.
	pixelColorPtr_ = effect_->GetVariableByName("pixelColor")->AsVector();

	return true;
}

/*
================
 FontShader::shutdownShader
================
*/
void FontShader::shutdownShader()
{	
	// FIXME: missing stuff here

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
 FontShader::outputShaderErrorMessage
================
*/
void FontShader::outputShaderErrorMessage( ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream ofs;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	ofs.open("shader-error.txt");

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
 FontShader::setShaderParameters
================
*/
void FontShader::setShaderParameters( D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView* texture, D3DXVECTOR4 pixelColor)
{
	// Set the world matrix variable inside the shader.
	worldMatrixPtr_->SetMatrix((float*)&worldMatrix);

	// Set the view matrix variable inside the shader.
	viewMatrixPtr_->SetMatrix((float*)&viewMatrix);

	// Set the projection matrix variable inside the shader.
	projectionMatrixPtr_->SetMatrix((float*)&projectionMatrix);

	// Bind the texture.
	texturePtr_->SetResource(texture);
	
	// Set the pixel color variable inside the shader with the pixelColor vector.
	pixelColorPtr_->SetFloatVector((float*)&pixelColor);
}

/*
================
 FontShader::renderShader
================
*/
void FontShader::renderShader( ID3D10Device* device, int indexCount)
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
