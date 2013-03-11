/*
================================
 object3d.h
 
 Created on: 11 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <d3d10.h>
#include <D3DX10.h>
#include <fstream>

#include "mesh.h"
#include "texture.h"
#include "moveable.h"
#include "common.h"


/*
================================
 
 Object3D

 A generic 3D object which can be loaded from .OBJ files.
 
================================
*/
class Object3D : public Moveable, Uncopyable
{
public:
	Object3D(void);	
	virtual ~Object3D(void);

	bool init(ID3D10Device* device, char*, WCHAR* textureFilename);
	void shutdown();
	void render(ID3D10Device* device);

	int getIndexCount() const;

	ID3D10ShaderResourceView* getTexture();

private:
	ID3D10Buffer* vertexBuffer_;
	ID3D10Buffer* indexBuffer_;
	int vertexCount_;
	int indexCount_;
	Texture* texture_;
	Mesh* mesh_;

	bool initBuffers(ID3D10Device* device);
	void renderBuffers(ID3D10Device* device);
	void shutdownBuffers();	
	bool loadTexture(ID3D10Device* device, WCHAR* filename);
	void releaseTexture();
	bool loadMesh(char* filename);
	bool loadMeshFromObj(char* filename);
	void releaseMesh();
};

