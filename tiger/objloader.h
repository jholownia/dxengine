/*
================================
 objloader.h

 Created on: 14 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include "mesh.h"
#include "common.h"

/*
================================
 
 ObjLoader

 A simple loader for .OBJ files.
 
================================
*/
class ObjLoader : Uncopyable
{
public:
	ObjLoader(void);
	~ObjLoader(void);

	bool init(char* filename);
	void shutdown();
	
	Mesh* createMesh(int& vertexCount);
	
private:
	struct Vector
	{
		float x, y, z;
	};

	struct Face
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

	Vector* vertices_;
	Vector* texcoords_;
	Vector* normals_;
	Face* faces_;

	int vertexIndex_;
	int texcoordIndex_;
	int normalIndex_;
	int faceIndex_;
	
	bool readFileCounts(char*, int&, int&, int&, int&);
	bool loadDataStructures(char*, int, int, int, int);
};

