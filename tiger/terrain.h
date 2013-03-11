/*
================================
 terrain.h
 
 Created on: 10 Jan 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#pragma once

#include <d3d10.h>
#include <D3DX10math.h>
#include <stdio.h>

#include "texture.h"
#include "vertex.h"
#include "common.h"


// How many times are we repeating the same texture over the entire terrain
const int TEXTURE_REPEAT = 8;


/*
================================
 
 Terrain

 Generates textured terrain mesh from depth image.
 
================================
*/
class Terrain : Uncopyable
{
	struct HeightMap
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct TerrainNormal
	{
		float x, y, z;
	};

public:
	Terrain(void);
	~Terrain(void);

	bool init(ID3D10Device*, char*, WCHAR*);
	void shutdown();	

	int getIndexCount() const;
	int getVertexCount() const;
	ID3D10ShaderResourceView* getTexture();

	void copyVertexArray(void* );

private:			
	int width_;
	int height_;
	int vertexCount_;
	int indexCount_;
	HeightMap* heightMap_;
	Texture* texture_;
	Vertex* vertices_;

	bool initBuffers(ID3D10Device*);
	void shutdownBuffers();

	bool loadHeightMap(char *);
	void normalizeHeightMap();
	void shutdownHeightMap();

	bool calculateNormals();

	void calculateTextureCoordinates();
	bool loadTexture(ID3D10Device*, WCHAR*);
	void releaseTexture();
};

