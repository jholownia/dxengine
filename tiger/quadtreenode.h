/*
================================
 quadtreenode.h
 
 Created on: 11 Jan 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#pragma once

#include "vector"

struct ID3D10Buffer;
/*
================================
 
 QuadTreeNode
 
================================
*/
struct QuadtreeNode
{
	struct Vector 
	{
		float x, y, z;
	};
	
	float positionX, positionZ;
	float width;
	int triangleCount;
	ID3D10Buffer* vertexBuffer;
	ID3D10Buffer* indexBuffer;
	Vector* vertexArray;
	QuadtreeNode* nodes[4];
};