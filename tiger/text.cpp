/*
================================
 text.cpp
 
 Created on: 17 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "text.h"

/*
================
 Text::Text
================
*/
Text::Text(void) :
	font_       (NULL),
	fontShader_ (NULL),
	sentence1_  (NULL),
	sentence2_  (NULL),
	sentence3_  (NULL)
{

}

/*
================
 Text::~Text
================
*/
Text::~Text(void)
{

}

/*
================
 Text::init
================
*/
bool Text::init( ID3D10Device* device, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
{
	bool result;

	screenWidth_ = screenWidth;
	screenHeight_ = screenHeight;
	baseViewMatrix_ = baseViewMatrix;
	
	// Initialize font object
	font_ = new Font;
	result = font_->init(device, "data/fontdata.txt", L"data/font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize font object.", L"Error", MB_OK);
		return false;
	}

	// Initialize font shader object
	fontShader_ = new FontShader;
	result = fontShader_->init(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize font shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize sentences (FIXME: change to std::vector and iterate)
	result = initSentence(&sentence1_, 32, device);
	if (!result)
	{
		return false;
	}
	
	result = updateSentence(sentence1_, "Flying Tiger 0.12", 15, 15, 1.0f, 0.0f, 0.0f);
	if (!result)
	{
		return false;
	}

	result = initSentence(&sentence2_, 16, device);
	if (!result)
	{
		return false;
	}

	result = updateSentence(sentence2_, "Press Esc", 15, 35, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = initSentence(&sentence3_, 32, device);
	if (!result)
	{
		return false;
	}

	return true;
}

/*
================
 Text::shutdown
================
*/
void Text::shutdown()
{
	releaseSentence(&sentence1_);
	releaseSentence(&sentence2_);
	releaseSentence(&sentence3_);

	if (fontShader_)
	{
		fontShader_->shutdown();
		delete fontShader_;
		fontShader_ = NULL;
	}

	if (font_)
	{
		font_->shutdown();
		delete font_;
		font_ = NULL;
	}
}

/*
================
 Text::render
================
*/
void Text::render( ID3D10Device* device, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	renderSentence(device, sentence1_, worldMatrix, orthoMatrix);
	renderSentence(device, sentence2_, worldMatrix, orthoMatrix);
	renderSentence(device, sentence3_, worldMatrix, orthoMatrix);
}

/*
================
 Text::initSentence

 The InitializeSentence function creates a SentenceType with an empty vertex buffer which will be used to store and render sentences. 
 The maxLength input parameter determines how large the vertex buffer will be. All sentences have a vertex and index buffer associated 
 with them which is initialized first in this function.
================
*/
bool Text::initSentence( SentenceType** sentence, int maxLength, ID3D10Device* device )
{
	VertexType* vertices;
	unsigned long* indices;
	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	
	// Create a new sentence object.
	*sentence = new SentenceType;
	
	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	
	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	
	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for(int i=0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}
	
	// During the creation of the vertex buffer description for the sentence we set the Usage type to dynamic as we may want to change the contents of the sentence at any time.

	// Set up the description of the _dynamic_ vertex buffer.
	vertexBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;    // !
	vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}
	
	// The index buffer is setup as a normal static buffer since the contents will never need to change.

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete [] indices;
	indices = 0;

	return true;
}

/*
================
 Text::updateSentence
================
*/
bool Text::updateSentence( SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	void* verticesPtr;
	HRESULT result;
	
	// Set the color and size of the sentence.

	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	
	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));
	
	// Calculate the starting location to draw the sentence on the screen at.

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((screenWidth_ / 2) * -1) + positionX);
	drawY = (float)((screenHeight_ / 2) - positionY);
	
	// Build the vertex array using the FontClass and the sentence information.

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	font_->buildVertexArray((void*)vertices, text, drawX, drawY);
	
	// Copy the vertex array information into the sentence vertex buffer.
	
	// Initialize the vertex buffer pointer to null first.
	verticesPtr = 0;

	// Lock the vertex buffer.
	result = sentence->vertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&verticesPtr);
	if(FAILED(result))
	{
		return false;
	}

	// Copy the vertex array into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	sentence->vertexBuffer->Unmap();

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}

/*
================
 Text::releaseSentence
================
*/
void Text::releaseSentence( SentenceType** sentence)
{
	if(*sentence)
	{
		// Release the sentence vertex buffer.
		if((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = NULL;
		}

		// Release the sentence index buffer.
		if((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = NULL;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}
}

/*
================
 Text::renderSentence
================
*/
void Text::renderSentence( ID3D10Device* device, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix )
{		
	// Set the vertex buffer stride and offset
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// Set the vertexBuffer to active in the input assembler so it can be rendered
	device->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer
	device->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set primitive topology
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Get color
	D3DXVECTOR4 pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render text using font shader
	fontShader_->render(device, sentence->indexCount, worldMatrix, baseViewMatrix_, orthoMatrix, font_->getTexture(), pixelColor);
}

/*
================
 Text::setMousePosition

 Currently unused.
================
*/
bool Text::setMousePosition( int mouseX, int mouseY)
{
	char tempString[16];
	char mouseString[16];
	 
	_itoa_s(mouseX, tempString, 10);
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	bool result = updateSentence(sentence1_, mouseString, 15, 45, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	_itoa_s(mouseY, tempString, 10);
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	return updateSentence(sentence2_, mouseString, 15, 60, 1.0f, 1.0f, 1.0f);	
}

/*
================
 Text::setFps
================
*/
bool Text::setFps( int fps )
{
	char tempString[16];
	char fpsString[16];
	
	if (fps > 9999)
	{
		fps = 9999;
	}

	_itoa_s(fps, tempString, 10);
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	return updateSentence(sentence1_, fpsString, 15, 15, 1.0f, 1.0f, 1.0f);
}

/*
================
 Text::setCpu
================
*/
bool Text::setCpu( int cpu )
{
	char tempString[16];
	char cpuString[16];

	_itoa_s(cpu, tempString, 10);
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	return updateSentence(sentence2_, cpuString, 15, 30, 1.0f, 1.0f, 1.0f);
}

/*
================
 Text::setPolyCount
================
*/
bool Text::setPolyCount( int count )
{
	char tempString[32];
	char polyString[32];

	if (count > 999999999)
	{
		count = 999999999;
	}

	_itoa_s(count, tempString, 10);
	strcpy_s(polyString, "Polygons: ");
	strcat_s(polyString, tempString);

	return updateSentence(sentence3_, polyString, 15, 45, 0.0f, 1.0f, 0.0f);
}
