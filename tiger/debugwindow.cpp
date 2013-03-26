/*
================================
 debugwindow.cpp
 
 Created on: 20 Mar 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#include "debugwindow.h"


/*
================
 DebugWindow::DebugWindow
================
*/
DebugWindow::DebugWindow(void) :
	vertexBuffer_ (NULL),
	indexBuffer_  (NULL)
{

}

/*
================
 DebugWindow::~DebugWindow
================
*/
DebugWindow::~DebugWindow(void)
{
}

/*
================
 DebugWindow::init
================
*/
bool DebugWindow::init( ID3D10Device* device, int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight)
{
	// Store the screen size.
	screenWidth_ = screenWidth;
	screenHeight_ = screenHeight;

	// Store the size in pixels that this bitmap should be rendered at.
	bitmapWidth_ = bitmapWidth;
	bitmapHeight_ = bitmapHeight;

	// Initialize the previous rendering position to negative one.
	previousPosX_ = -1;
	previousPosY_ = -1;

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	return initBuffers(device);
}

/*
================
 DebugWindow::shutdown
================
*/
void DebugWindow::shutdown()
{
	shutdownBuffers();
}

/*
================
 DebugWindow::render
================
*/
bool DebugWindow::render( ID3D10Device* device, int x, int y)
{	
	// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
	bool result = updateBuffers(x, y);
	if(!result)
	{
		return false;
	}

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	renderBuffers(device);

	return true;
}

/*
================
 DebugWindow::getIndexCount
================
*/
int DebugWindow::getIndexCount()
{
	return indexCount_;
}

/*
================
 DebugWindow::DebugWindow
================
*/
bool DebugWindow::initBuffers( ID3D10Device* device )
{		
	// Set the number of vertices in the vertex array.
	vertexCount_ = 6;

	// Set the number of indices in the index array.
	indexCount_ = vertexCount_;

	// Create the vertex array.
	VertexType* vertices = new VertexType[vertexCount_];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	unsigned long* indices = new unsigned long[indexCount_];
	if(!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * vertexCount_));

	// Load the index array with data.
	for(int i=0; i < indexCount_; i++)
	{
		indices[i] = i;
	}

	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D10_SUBRESOURCE_DATA vertexData, indexData;

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount_;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;

	// Now finally create the vertex buffer.
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer_);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount_;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer_);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

/*
================
 DebugWindow::shutdownBuffers
================
*/
void DebugWindow::shutdownBuffers()
{
	if (indexBuffer_)
	{
		indexBuffer_->Release();
		indexBuffer_ = NULL;
	}

	if (vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = NULL;
	}
}

/*
================
 DebugWindow::updateBuffers
================
*/
bool DebugWindow::updateBuffers( int positionX, int positionY )
{
	float left, right, top, bottom;
	VertexType* vertices;
	void* verticesPtr;
	HRESULT result;


	// If the position we are rendering this bitmap to has not changed then don't update the vertex buffer since it
	// currently has the correct parameters.
	if((positionX == previousPosX_) && (positionY == previousPosY_))
	{
		return true;
	}

	// If it has changed then update the position it is being rendered to.
	previousPosX_ = positionX;
	previousPosY_ = positionY;

	// Calculate the screen coordinates of the left side of the bitmap.
	left = (float)((screenWidth_ / 2) * -1) + (float)positionX;

	// Calculate the screen coordinates of the right side of the bitmap.
	right = left + (float)bitmapWidth_;

	// Calculate the screen coordinates of the top of the bitmap.
	top = (float)(screenHeight_ / 2) - (float)positionY;

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - (float)bitmapHeight_;

	// Create the vertex array.
	vertices = new VertexType[vertexCount_];
	if(!vertices)
	{
		return false;
	}

	// Load the vertex array with data.
	// First triangle.
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	// Initialize the vertex buffer pointer to null first.
	verticesPtr = 0;

	// Lock the vertex buffer.
	result = vertexBuffer_->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&verticesPtr);
	if(FAILED(result))
	{
		return false;
	}

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * vertexCount_));

	// Unlock the vertex buffer.
	vertexBuffer_->Unmap();

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}

/*
================
 DebugWindow::DebugWindow
================
*/
void DebugWindow::renderBuffers( ID3D10Device* device )
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(VertexType); 
	unsigned int offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	device->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	device->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
