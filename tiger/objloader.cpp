/*
================================
 objloader.cpp

 Created on: 14 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "objloader.h"

#include <fstream>


/*
================
 ObjLoader::ObjLoader
================
*/
ObjLoader::ObjLoader(void) :
	vertices_      (NULL),
	texcoords_     (NULL),
	normals_       (NULL),
	faces_         (NULL),
	vertexIndex_   (0),
	texcoordIndex_ (0),
	normalIndex_   (0),
	faceIndex_     (0)
{

}

/*
================
 ObjLoader::~ObjLoader
================
*/
ObjLoader::~ObjLoader(void)
{

}

/*
================
 ObjLoader::init
================
*/
bool ObjLoader::init( char* filename )
{
	bool result;	
	int vertexCount = 0;
	int textureCount = 0;
	int normalCount = 0;
	int faceCount = 0;	

	result = readFileCounts(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return false;
	}

	result = loadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return false;
	}

	return true;	
}

/*
================
 ObjLoader::readFileCounts

 Goes through the file and counts the number of vertices, textures, normals and faces
 which are then used to initialize buffers properly.
================
*/
bool ObjLoader::readFileCounts( char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount )
{
	std::ifstream ifs;
	char input;

	ifs.open(filename);
	if (ifs.fail())
	{
		return false;
	}

	// Read the whole file and count the stuff
	ifs.get(input);
	while (!ifs.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			ifs.get(input);
			
			if (input == ' ')
			{
				++vertexCount;
			}
			else if (input == 't')
			{
				++textureCount;
			}
			else if (input == 'n')
			{
				++normalCount;
			}
		}
		// If the line starts with 'f' then increment the face count.
		if(input == 'f')
		{
			ifs.get(input);

			if(input == ' ') 
			{ 
				++faceCount; 
			}
		}

		// Otherwise read in the remainder of the line.
		while(input != '\n')
		{
			ifs.get(input);
		}

		// Start reading the beginning of the next line.
		ifs.get(input);
	}

	// Close the file.
	ifs.close();

	return true;
}

/*
================
 ObjLoader::loadDataStructures

 Loads data into buffers.
================
*/
bool ObjLoader::loadDataStructures( char* filename, int vertexCount, int textureCount, int normalCount, int faceCount )
{
	vertices_ = new Vector[vertexCount];
	texcoords_ = new Vector[textureCount];
	normals_ = new Vector[normalCount];
	faces_ = new Face[faceCount];

	char input;
	char input2;
	
	std::ifstream ifs;

	ifs.open(filename);
	if (ifs.fail())
	{
		return false;
	}

	// Read the data in and convert RH coordinates to LH
	ifs.get(input);
	while (!ifs.eof())
	{
		if(input == 'v')
		{
			ifs.get(input);

			// Read in the vertices_.
			if(input == ' ') 
			{ 
				ifs >> vertices_[vertexIndex_].x >> vertices_[vertexIndex_].y >> vertices_[vertexIndex_].z;

				// Invert the Z vertex to change to left hand system.
				vertices_[vertexIndex_].z = vertices_[vertexIndex_].z * -1.0f;
				vertexIndex_++; 
			}

			// Read in the texture uv coordinates.
			if(input == 't') 
			{ 
				ifs >> texcoords_[texcoordIndex_].x >> texcoords_[texcoordIndex_].y;

				// Invert the V texture coordinates to left hand system.
				texcoords_[texcoordIndex_].y = 1.0f - texcoords_[texcoordIndex_].y;
				texcoordIndex_++; 
			}

			// Read in the normals_.
			if(input == 'n') 
			{ 
				ifs >> normals_[normalIndex_].x >> normals_[normalIndex_].y >> normals_[normalIndex_].z;

				// Invert the Z normal to change to left hand system.
				normals_[normalIndex_].z = normals_[normalIndex_].z * -1.0f;
				normalIndex_++; 
			}
		}

		// Read in the faces_.
		if(input == 'f') 
		{
			ifs.get(input);
			if(input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				ifs >> faces_[faceIndex_].vIndex3 >> input2 >> faces_[faceIndex_].tIndex3 >> input2 >> faces_[faceIndex_].nIndex3
					>> faces_[faceIndex_].vIndex2 >> input2 >> faces_[faceIndex_].tIndex2 >> input2 >> faces_[faceIndex_].nIndex2
					>> faces_[faceIndex_].vIndex1 >> input2 >> faces_[faceIndex_].tIndex1 >> input2 >> faces_[faceIndex_].nIndex1;
				faceIndex_++;
			}
		}

		// Read in the remainder of the line.
		while(input != '\n')
		{
			ifs.get(input);
		}

		// Start reading the beginning of the next line.
		ifs.get(input);
	}

	ifs.close();

	return true;
}

/*
================
 ObjLoader::shutdown
================
*/
void ObjLoader::shutdown()
{
	delete[] vertices_;
	delete[] texcoords_;
	delete[] normals_;
	delete[] faces_;
}

/*
================
 ObjLoader::createMesh
================
*/
Mesh* ObjLoader::createMesh( int& vertexCount )
{
	int vIndex;
	int tIndex;
	int nIndex;

	vertexCount = faceIndex_ * 3;
	Mesh* mesh = new Mesh[vertexCount];
	
	int m = 0;

	for (int i = 0; i < faceIndex_; ++i)
	{
		vIndex = faces_[i].vIndex1 - 1;
		tIndex = faces_[i].tIndex1 - 1;
		nIndex = faces_[i].nIndex1 - 1;

		mesh[m].x = vertices_[vIndex].x;
		mesh[m].y = vertices_[vIndex].y;
		mesh[m].z = vertices_[vIndex].z;

		mesh[m].tu = texcoords_[tIndex].x;
		mesh[m].tv = texcoords_[tIndex].y;

		mesh[m].nx = normals_[nIndex].x;
		mesh[m].ny = normals_[nIndex].y;
		mesh[m].nz = normals_[nIndex].z;

		++m;

		vIndex = faces_[i].vIndex2 - 1;
		tIndex = faces_[i].tIndex2 - 1;
		nIndex = faces_[i].nIndex2 - 1;

		mesh[m].x = vertices_[vIndex].x;
		mesh[m].y = vertices_[vIndex].y;
		mesh[m].z = vertices_[vIndex].z;

		mesh[m].tu = texcoords_[tIndex].x;
		mesh[m].tv = texcoords_[tIndex].y;

		mesh[m].nx = normals_[nIndex].x;
		mesh[m].ny = normals_[nIndex].y;
		mesh[m].nz = normals_[nIndex].z;

		++m;

		vIndex = faces_[i].vIndex3 - 1;
		tIndex = faces_[i].tIndex3 - 1;
		nIndex = faces_[i].nIndex3 - 1;

		mesh[m].x = vertices_[vIndex].x;
		mesh[m].y = vertices_[vIndex].y;
		mesh[m].z = vertices_[vIndex].z;

		mesh[m].tu = texcoords_[tIndex].x;
		mesh[m].tv = texcoords_[tIndex].y;

		mesh[m].nx = normals_[nIndex].x;
		mesh[m].ny = normals_[nIndex].y;
		mesh[m].nz = normals_[nIndex].z;

		++m;
	}

	return mesh;
}
