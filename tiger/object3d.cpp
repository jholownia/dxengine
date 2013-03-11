/*
================================
 object3d.h
 
 Created on: 11 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "object3d.h"
#include "vertex.h"
#include "objloader.h"

#include <iostream>
#include <string>


/*
================
 Object3D::Object3D
================
*/
Object3D::Object3D(void) :
	vertexBuffer_ (NULL),
	indexBuffer_  (NULL),
	texture_      (NULL),
	mesh_         (NULL)
{

}

/*
================
 Object3D::~Object3D
================
*/
Object3D::~Object3D(void)
{

}

/*
================
 Object3D::init

 Creates a 3d object given direct 3d device, .obj mesh filename and texture filename.
================
*/
bool Object3D::init( ID3D10Device* device, char* meshFilename, WCHAR* textureFilename )
{
	bool result;

	// Load mesh
	{
		std::string s(meshFilename);

		if (s.find(".obj") == std::string::npos)
		{
			result = loadMesh(meshFilename);
		}
		else
		{
			result = loadMeshFromObj(meshFilename);
		}
	}	
	if (!result)
	{
		return false;
	}
	
	result = initBuffers(device);
	if (!result)
	{
		return false;
	}

	result = loadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

/*
================
 Object3D::shutdown
================
*/
void Object3D::shutdown()
{
	releaseTexture();
	shutdownBuffers();
	releaseMesh();
}

/*
================
 Object3D::render
================
*/
void Object3D::render( ID3D10Device* device )
{
	renderBuffers(device);
}

/*
================
 Object3D::getIndexCount
================
*/
int Object3D::getIndexCount() const
{
	return indexCount_;
}

/*
================
 Object3D::getTexture
================
*/
ID3D10ShaderResourceView* Object3D::getTexture()
{
	return texture_->getTexture();
}

/*
================
 Object3D::initBuffers

 Creates vertex and index buffers.
================
*/
bool Object3D::initBuffers( ID3D10Device* device )
{
	Vertex* vertices;
	unsigned long* indices;
	D3D10_BUFFER_DESC vertexBufferDesc;
	D3D10_BUFFER_DESC indexBufferDesc;
	D3D10_SUBRESOURCE_DATA vertexData;
	D3D10_SUBRESOURCE_DATA indexData;
	HRESULT result;

	vertices = new Vertex[vertexCount_];
	indices = new unsigned long[indexCount_];

	for (int i = 0; i < vertexCount_; i++)
	{
		vertices[i].position = D3DXVECTOR3(mesh_[i].x, mesh_[i].y, mesh_[i].z);
		vertices[i].texture = D3DXVECTOR2(mesh_[i].tu, mesh_[i].tv);
		vertices[i].normal = D3DXVECTOR3(mesh_[i].nx, mesh_[i].ny, mesh_[i].nz);

		indices[i] = i;
	}

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertexCount_;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer_);
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
	vertices = NULL;

	delete [] indices;
	indices = NULL;

	return true;
}

/*
================
 Object3D::renderBuffers
================
*/
void Object3D::renderBuffers( ID3D10Device* device )
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(Vertex);
	offset = 0;

	device->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	device->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/*
================
 Object3D::shutdownBuffers
================
*/
void Object3D::shutdownBuffers()
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
 Object3D::loadTexture
================
*/
bool Object3D::loadTexture( ID3D10Device* device, WCHAR* filename )
{
	texture_ = new Texture;

	return texture_->init(device, filename);
}

/*
================
 Object3D::releaseTexture
================
*/
void Object3D::releaseTexture()
{
	if (texture_)
	{
		texture_->shutdown();
		delete texture_;
		texture_ = NULL;
	}
}

/*
================
 Object3D::loadMesh

 Loads mesh data from a simple text file with
 the following format:
  
 Vertex Count: nnn

 Data:
 x, y, z, tu, tv, nx, ny, nz
 x, y, z, tu, tv, nx, ny, nz
 ...
================
*/
bool Object3D::loadMesh( char* filename )
{
	std::ifstream ifs;
	
	ifs.open(filename);
	if (ifs.fail())
	{
		return false;
	}

	char input;
	
	// Get to vertex count
	ifs.get(input);
	while(input != ':')
	{
		ifs.get(input);
	}

	// read vertex count
	ifs >> vertexCount_;
	indexCount_ = vertexCount_;

	// Create mesh
	mesh_ = new Mesh[vertexCount_];

	// Read to the beginning of the data
	ifs.get(input);
	while(input != ':')
	{
		ifs.get(input);
	}
	ifs.get(input);
	ifs.get(input);

	// Read the data
	for (int i = 0; i < vertexCount_; ++i)
	{
		ifs >> mesh_[i].x >> mesh_[i].y >> mesh_[i].z;
		ifs >> mesh_[i].tu >> mesh_[i].tv;
		ifs >> mesh_[i].nx >> mesh_[i].ny >> mesh_[i].nz;		
	}

	ifs.close();

	return true;
}

/*
================
 Object3D::loadMeshFromObj
================
*/
bool Object3D::loadMeshFromObj( char* filename )
{
	bool result;
	
	ObjLoader loader;
	result = loader.init(filename);
	mesh_ = loader.createMesh(vertexCount_);
	loader.shutdown();

	indexCount_ = vertexCount_;

	return result;
}

/*
================
 Object3D::releaseMesh
================
*/
void Object3D::releaseMesh()
{	
	delete mesh_;
	mesh_ = NULL;	
}


