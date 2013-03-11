/*
================================
 terrain.cpp
 
 Created on: 10 Jan 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#include "terrain.h"

/*
================
 Terrain::Terrain
================
*/
Terrain::Terrain(void) :
	width_        (0),
	height_       (0),
	vertexCount_  (0),
	indexCount_   (0),	
	heightMap_    (NULL),
	texture_      (NULL),
	vertices_     (NULL)
{

}

/*
================
 Terrain::~Terrain
================
*/
Terrain::~Terrain(void)
{

}

/*
================
 Terrain::init
================
*/
bool Terrain::init( ID3D10Device* device, char* heightmapFilename, WCHAR* textureFilename )
{
	bool result;
	
	result = loadHeightMap(heightmapFilename);
	if (!result)
	{
		return false;
	}

	normalizeHeightMap();

	result = calculateNormals();
	if (!result)
	{
		return true;
	}
	
	calculateTextureCoordinates();

	result = loadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	result = initBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

/*
================
 Terrain::shutdown
================
*/
void Terrain::shutdown()
{
	releaseTexture();
	shutdownBuffers();
	shutdownHeightMap();
}

/*
================
 Terrain::getIndexCount
================
*/
int Terrain::getIndexCount() const
{
	return indexCount_;
}

/*
================
 Terrain::initBuffers
================
*/
bool Terrain::initBuffers( ID3D10Device* device )
{
	// TerrainVertex* vertices;   change to vector
			
	vertexCount_ = (width_ - 1) * (height_ - 1) * 6;
	indexCount_ = vertexCount_;

	vertices_ = new Vertex[vertexCount_];
	
	int index = 0;
	int index1, index2, index3, index4;	
	float tu, tv;

	for (int j = 0; j < height_ - 1; ++j)
	{
		for (int i = 0; i < width_ - 1; ++i)
		{
			index1 = (height_ * j) + i;          // Bottom left.
			index2 = (height_ * j) + (i+1);      // Bottom right.
			index3 = (height_ * (j+1)) + i;      // Upper left.
			index4 = (height_ * (j+1)) + (i+1);  // Upper right.

			// Upper left.
			tv = heightMap_[index3].tv;

			// Modify the texture coordinates to cover the top edge.
			if(tv == 1.0f) { tv = 0.0f; }

			vertices_[index].position = D3DXVECTOR3(heightMap_[index3].x, heightMap_[index3].y, heightMap_[index3].z);
			vertices_[index].texture = D3DXVECTOR2(heightMap_[index3].tu, tv);
			vertices_[index].normal = D3DXVECTOR3(heightMap_[index3].nx, heightMap_[index3].ny, heightMap_[index3].nz);		
			index++;

			// Upper right.
			tu = heightMap_[index4].tu;
			tv = heightMap_[index4].tv;

			// Modify the texture coordinates to cover the top and right edge.
			if(tu == 0.0f) { tu = 1.0f; }
			if(tv == 1.0f) { tv = 0.0f; }

			vertices_[index].position = D3DXVECTOR3(heightMap_[index4].x, heightMap_[index4].y, heightMap_[index4].z);
			vertices_[index].texture = D3DXVECTOR2(tu, tv);
			vertices_[index].normal = D3DXVECTOR3(heightMap_[index4].nx, heightMap_[index4].ny, heightMap_[index4].nz);			
			index++;

			// Bottom left.
			vertices_[index].position = D3DXVECTOR3(heightMap_[index1].x, heightMap_[index1].y, heightMap_[index1].z);
			vertices_[index].texture = D3DXVECTOR2(heightMap_[index1].tu, heightMap_[index1].tv);
			vertices_[index].normal = D3DXVECTOR3(heightMap_[index1].nx, heightMap_[index1].ny, heightMap_[index1].nz);			
			index++;

			// Bottom left.
			vertices_[index].position = D3DXVECTOR3(heightMap_[index1].x, heightMap_[index1].y, heightMap_[index1].z);
			vertices_[index].texture = D3DXVECTOR2(heightMap_[index1].tu, heightMap_[index1].tv);
			vertices_[index].normal = D3DXVECTOR3(heightMap_[index1].nx, heightMap_[index1].ny, heightMap_[index1].nz);		
			index++;

			// Upper right.
			tu = heightMap_[index4].tu;
			tv = heightMap_[index4].tv;

			// Modify the texture coordinates to cover the top and right edge.
			if(tu == 0.0f) { tu = 1.0f; }
			if(tv == 1.0f) { tv = 0.0f; }

			vertices_[index].position = D3DXVECTOR3(heightMap_[index4].x, heightMap_[index4].y, heightMap_[index4].z);
			vertices_[index].texture = D3DXVECTOR2(tu, tv);
			vertices_[index].normal = D3DXVECTOR3(heightMap_[index4].nx, heightMap_[index4].ny, heightMap_[index4].nz);		
			index++;

			// Bottom right.
			tu = heightMap_[index2].tu;

			// Modify the texture coordinates to cover the right edge.
			if(tu == 0.0f) { tu = 1.0f; }

			vertices_[index].position = D3DXVECTOR3(heightMap_[index2].x, heightMap_[index2].y, heightMap_[index2].z);
			vertices_[index].texture = D3DXVECTOR2(tu, heightMap_[index2].tv);
			vertices_[index].normal = D3DXVECTOR3(heightMap_[index2].nx, heightMap_[index2].ny, heightMap_[index2].nz);		
			index++;
		}
	}
	
	return true;
}

/*
================
 Terrain::shutdownBuffers
================
*/
void Terrain::shutdownBuffers()
{
	if (vertices_)
	{
		delete[] vertices_;
		vertices_ = NULL;
	}
}

/*
================
 Terrain::loadHeightMap
================
*/
bool Terrain::loadHeightMap( char* filename )
{
	// We use bitmaps because they store file dimensions
	
	FILE* filePtr;

	if (fopen_s(&filePtr, filename, "rb") != 0)
	{
		return false;
	}

	
	BITMAPFILEHEADER bitmapFileHeader;
	unsigned int count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	BITMAPINFOHEADER bitmapInfoHeader;
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Get the size of the terrain
	width_ = bitmapInfoHeader.biWidth;
	height_ = bitmapInfoHeader.biHeight;

	int imageSize = width_ * height_ * 3;

	unsigned char* bitmapImage = new unsigned char[imageSize];

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if(count != imageSize)
	{
		// return false;
	}

	if (fclose(filePtr) != 0)
	{
		return false;
	}

	// FIXME: This part can be optimized with pointer arithmetic
	heightMap_ = new HeightMap[width_ * height_];

	int k = 0;
	int index;
	unsigned char height;

	for (int j = 0; j < height_; ++j)
	{
		for (int i = 0; i < width_; ++i)
		{
			height = bitmapImage[k];
			index = (height_ * j) + i;

			heightMap_[index].x = (float)i;
			heightMap_[index].y = (float)height;
			heightMap_[index].z = (float)j;

			k+=3;
		}
	}

	delete[] bitmapImage;
	bitmapImage = NULL;

	return true;
}

/*
================
 Terrain::normalizeHeightMap
================
*/
void Terrain::normalizeHeightMap()
{
	// FIXME: use HeightMap pointer instead
	 
	for (int j = 0; j < height_; ++j)
	{
		for (int i = 0; i < width_; ++i)
		{
			heightMap_[(height_ * j) + i].y /= 15.0f;
		}
	}
}

/*
================
 Terrain::shutdownHeightMap
================
*/
void Terrain::shutdownHeightMap()
{
	delete heightMap_;
	heightMap_ = NULL;
}

/*
================
 Terrain::calculateNormals
================
*/
bool Terrain::calculateNormals()
{
	// FIXME: this can be be optimized

	int i, j, index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	TerrainNormal* normals;


	// Create a temporary array to hold the un-normalized normal vectors.
	normals = new TerrainNormal[(height_ - 1) * (width_-1)];
	
	// Go through all the faces in the mesh and calculate their normals.
	for(j=0; j<(height_-1); j++)
	{
		for(i=0; i<(width_-1); i++)
		{
			index1 = (j * height_) + i;
			index2 = (j * height_) + (i+1);
			index3 = ((j+1) * height_) + i;

			// Get three vertices from the face.
			vertex1[0] = heightMap_[index1].x;
			vertex1[1] = heightMap_[index1].y;
			vertex1[2] = heightMap_[index1].z;

			vertex2[0] = heightMap_[index2].x;
			vertex2[1] = heightMap_[index2].y;
			vertex2[2] = heightMap_[index2].z;

			vertex3[0] = heightMap_[index3].x;
			vertex3[1] = heightMap_[index3].y;
			vertex3[2] = heightMap_[index3].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (height_-1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for(j=0; j<height_; j++)
	{
		for(i=0; i<width_; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if(((i-1) >= 0) && ((j-1) >= 0))
			{
				index = ((j-1) * (height_-1)) + (i-1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if((i < (width_-1)) && ((j-1) >= 0))
			{
				index = ((j-1) * (height_-1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if(((i-1) >= 0) && (j < (height_-1)))
			{
				index = (j * (height_-1)) + (i-1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if((i < (width_-1)) && (j < (height_-1)))
			{
				index = (j * (height_-1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * height_) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			heightMap_[index].nx = (sum[0] / length);
			heightMap_[index].ny = (sum[1] / length);
			heightMap_[index].nz = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	delete [] normals;
	normals = 0;

	return true;
}

/*
================
 Terrain::getTexture
================
*/
ID3D10ShaderResourceView* Terrain::getTexture()
{
	return texture_->getTexture();
}

/*
================
 Terrain::calculateTextureCoordinates
================
*/
void Terrain::calculateTextureCoordinates()
{
	int incrementCount, i, j, tuCount, tvCount;
	float incrementValue, tuCoordinate, tvCoordinate;


	// Calculate how much to increment the texture coordinates by.
	incrementValue = (float)TEXTURE_REPEAT / (float)width_;

	// Calculate how many times to repeat the texture.
	incrementCount = width_ / TEXTURE_REPEAT;

	// Initialize the tu and tv coordinate values.
	tuCoordinate = 0.0f;
	tvCoordinate = 1.0f;

	// Initialize the tu and tv coordinate indexes.
	tuCount = 0;
	tvCount = 0;

	// Loop through the entire height map and calculate the tu and tv texture coordinates for each vertex.
	for(j=0; j<height_; j++)
	{
		for(i=0; i<width_; i++)
		{
			// Store the texture coordinate in the height map.
			heightMap_[(height_ * j) + i].tu = tuCoordinate;
			heightMap_[(height_ * j) + i].tv = tvCoordinate;

			// Increment the tu texture coordinate by the increment value and increment the index by one.
			tuCoordinate += incrementValue;
			tuCount++;

			// Check if at the far right end of the texture and if so then start at the beginning again.
			if(tuCount == incrementCount)
			{
				tuCoordinate = 0.0f;
				tuCount = 0;
			}
		}

		// Increment the tv texture coordinate by the increment value and increment the index by one.
		tvCoordinate -= incrementValue;
		tvCount++;

		// Check if at the top of the texture and if so then start at the bottom again.
		if(tvCount == incrementCount)
		{
			tvCoordinate = 1.0f;
			tvCount = 0;
		}
	}
}

/*
================
 Terrain::loadTexture
================
*/
bool Terrain::loadTexture( ID3D10Device* device, WCHAR* filename )
{
	texture_ = new Texture;

	return texture_->init(device, filename);
}

/*
================
 Terrain::releaseTexture
================
*/
void Terrain::releaseTexture()
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
 Terrain::getVertexCount
================
*/
int Terrain::getVertexCount() const
{
	return vertexCount_;
}

/*
================
 Terrain::copyVertexArray
================
*/
void Terrain::copyVertexArray( void* vertexList )
{
	// FIXME: use std::vector	
	memcpy (vertexList, vertices_, sizeof(Vertex) * vertexCount_);
}
