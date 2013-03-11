/*
================================
 quadtree.h
 
 Created on: 11 Jan	2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#pragma once

#include "terrain.h"
#include "frustum.h"
#include "terrainshader.h"
#include "vertex.h"
#include "quadtreenode.h"

const int MAX_TRIANGLES = 10000;


/*
================================
 
 Quadtree

 A quadtree data structure used for indexing and rendering terrain in chunks.
 Used together with frustum class makes sure that only visible part of the terrain
 is rendered.
 
================================
*/
class Quadtree : Uncopyable
{
public:
	Quadtree(void);
	~Quadtree(void);

	bool init(Terrain* terrain, ID3D10Device* device);
	void shutdown();
	void render(Frustum* frustum, ID3D10Device* device, TerrainShader* shader);

	int getDrawCount() const;
	bool getHeightAtPosition(float, float, float&);

private:
	int triangleCount_;
	int drawCount_;
	Vertex* vertexList_;
	QuadtreeNode* parentNode_;

	void calculateMeshDimensions(int, float&, float&, float&);
	void createNode(QuadtreeNode*, float, float, float, ID3D10Device*);
	int countTriangles(float, float, float);
	bool isTriangleContained(int, float, float, float);
	void releaseNode(QuadtreeNode*);
	void renderNode(QuadtreeNode*, Frustum*, ID3D10Device*, TerrainShader*);
	void findNode(QuadtreeNode*, float, float, float&);
	bool checkHeightOfTriangle(float, float, float&, float[3], float[3], float[3]);

};

