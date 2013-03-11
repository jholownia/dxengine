/*
================================
 quadtree.cpp

 Created on: 11 Jan	2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#include "quadtree.h"

/*
================
 Quadtree::Quadtree
================
*/
Quadtree::Quadtree(void) :
	triangleCount_ (0),
	drawCount_     (0),
	vertexList_    (NULL),
	parentNode_    (0)
{

}

/*
================
 Quadtree::~Quadtree
================
*/
Quadtree::~Quadtree(void)
{
	
}

/*
================
 Quadtree::init

 Builds the quadtree recursively.
================
*/
bool Quadtree::init( Terrain* terrain, ID3D10Device* device )
{
	int vertexCount = terrain->getVertexCount();
	triangleCount_ = vertexCount / 3;
	vertexList_ = new Vertex[vertexCount];

	terrain->copyVertexArray((void *) vertexList_);    // FIXME: use std::vectors

	// Calculate center x, z and width of the mesh
	float centerX, centerZ, width;
	calculateMeshDimensions(vertexCount, centerX, centerZ, width);

	// Create parent node
	parentNode_ = new QuadtreeNode;

	// Build the quadtree
	createNode(parentNode_, centerX, centerZ, width, device);

	// Delete vertex list, since the vertices are now in the nodes
	delete[] vertexList_;
	vertexList_ = NULL;

	return true;
}

/*
================
 Quadtree::shutdown
================
*/
void Quadtree::shutdown()
{
	if (parentNode_)
	{
		releaseNode(parentNode_);
		delete parentNode_;
		parentNode_ = NULL;
	}
}

/*
================
 Quadtree::render
================
*/
void Quadtree::render( Frustum* frustum, ID3D10Device* device, TerrainShader* shader )
{
	drawCount_ = 0;
	renderNode(parentNode_, frustum, device, shader);
}

/*
================
 Quadtree::getDrawCount

 Returns the number of drawn polygons.
================
*/
int Quadtree::getDrawCount() const
{
	return drawCount_;
}

/*
================
 Quadtree::calculateMeshDimensions
================
*/
void Quadtree::calculateMeshDimensions( int vertexCount, float& centerX, float& centerZ, float& meshWidth )
{
	// Find the center of the mesh
	centerX = 0.0f;
	centerZ = 0.0f;

	for (int i = 0; i < vertexCount; ++i)
	{
		centerX += vertexList_[i].position.x;
		centerZ += vertexList_[i].position.z;
	}

	centerX = centerX / (float)vertexCount;
	centerZ = centerZ / (float)vertexCount;

	// Find maximum width of the mesh
	float maxWidth = 0.0f;
	float maxDepth = 0.0f;

	float minWidth = fabs(vertexList_[0].position.x - centerX);
	float minDepth = fabs(vertexList_[0].position.z - centerZ);

	float width, depth;

	for (int i = 0; i < vertexCount; ++i)
	{
		width = fabs(vertexList_[i].position.x - centerX);
		depth = fabs(vertexList_[i].position.z - centerZ);

		if (width > maxWidth) { maxWidth = width; }
		if (depth > maxDepth) { maxDepth = depth; }
		if (width < minWidth) { minWidth = width; }
		if (depth < minDepth) { minDepth = depth; }
	}
	float maxX = (float)max(fabs(minWidth), fabs(maxWidth));
	float maxZ = (float)max(fabs(minDepth), fabs(maxDepth));

	meshWidth = max(maxX, maxZ) * 2.0f;
}

/*
================
 Quadtree::createNode

 Recursively creates quadtree nodes.
================
*/
void Quadtree::createNode( QuadtreeNode* node, float positionX, float positionZ, float width,ID3D10Device* device )
{
	node->positionX = positionX;
	node->positionZ = positionZ;
	node->width = width;
	node->triangleCount = 0;
	node->vertexBuffer = NULL;
	node->indexBuffer = NULL;
	node->vertexArray = NULL;
	for (int i = 0; i < 4; ++i)
	{
		node->nodes[i] = NULL;
	}

	int numTriangles = countTriangles(positionX, positionZ, width);
	
	// If node contains no triangles
	if (numTriangles == 0)
	{
		return;
	}

	// If node contains too many triangles split it into four new nodes
	if(numTriangles > MAX_TRIANGLES)
	{
		int count;
		float offsetX, offsetZ;

		for (int i = 0; i < 4; ++i)
		{
			offsetX = (((i % 2) < 1) ? -1.0f : 1.0f) * (width / 4.0f);
			offsetZ = (((i % 4) < 2) ? -1.0f : 1.0f) * (width / 4.0f);

			count = countTriangles((positionX + offsetX), (positionZ + offsetZ), (width / 2.0f));

			if (count > 0)
			{
				node->nodes[i] = new QuadtreeNode;
				createNode(node->nodes[i], (positionX + offsetX), (positionZ + offsetZ), (width / 2.0f), device);
			}
		}

		return;
	}

	// If node contains an appropriate number of triangles create vertex and index buffers
	node->triangleCount = numTriangles;
	int vertexCount = numTriangles * 3;

	Vertex* vertices = new Vertex[vertexCount];
	unsigned long* indices = new unsigned long[vertexCount];

	node->vertexArray = new QuadtreeNode::Vector[vertexCount];

	int index = 0;
	int vertexIndex;

	for (int i = 0; i < triangleCount_; ++i)
	{
		if (isTriangleContained(i, positionX, positionZ, width))
		{
			vertexIndex = i * 3;

			// Get the three vertices of this triangle from the vertex list.
			vertices[index].position = vertexList_[vertexIndex].position;
			vertices[index].texture = vertexList_[vertexIndex].texture;
			vertices[index].normal = vertexList_[vertexIndex].normal;
			indices[index] = index;
			
			node->vertexArray[index].x = vertexList_[vertexIndex].position.x;
			node->vertexArray[index].y = vertexList_[vertexIndex].position.y;
			node->vertexArray[index].z = vertexList_[vertexIndex].position.z;
						
			++index;
			++vertexIndex;

			vertices[index].position = vertexList_[vertexIndex].position;
			vertices[index].texture = vertexList_[vertexIndex].texture;
			vertices[index].normal = vertexList_[vertexIndex].normal;
			indices[index] = index;
			
			node->vertexArray[index].x = vertexList_[vertexIndex].position.x;
			node->vertexArray[index].y = vertexList_[vertexIndex].position.y;
			node->vertexArray[index].z = vertexList_[vertexIndex].position.z;
			
			++index;						
			++vertexIndex;

			vertices[index].position = vertexList_[vertexIndex].position;
			vertices[index].texture = vertexList_[vertexIndex].texture;
			vertices[index].normal = vertexList_[vertexIndex].normal;
			indices[index] = index;
			
			node->vertexArray[index].x = vertexList_[vertexIndex].position.x;
			node->vertexArray[index].y = vertexList_[vertexIndex].position.y;
			node->vertexArray[index].z = vertexList_[vertexIndex].position.z;
			
			++index;
		}
	}
	
	// Set up the description of the vertex buffer.
	D3D10_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;	

	// Give the subresource structure a pointer to the vertex data.
	D3D10_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &node->vertexBuffer);

	// Set up the description of the index buffer.
	D3D10_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * vertexCount;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	
	// Give the subresource structure a pointer to the index data.
	D3D10_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &node->indexBuffer);

	// Release the vertex and index arrays now that the data is stored in the buffers in the node.
	delete [] vertices;
	vertices = NULL;

	delete [] indices;
	indices = NULL;
}

/*
================
 Quadtree::countTriangles
================
*/
int Quadtree::countTriangles( float positionX, float positionZ, float width )
{
	int count = 0;

	for (int i = 0; i < triangleCount_; ++i)
	{
		if (isTriangleContained(i, positionX, positionZ, width))
		{
			++count;
		}
	}

	return count;
}

/*
================
 Quadtree::isTriangleContained
================
*/
bool Quadtree::isTriangleContained( int index, float positionX, float positionZ, float width)
{
	float radius = width / 2.0f;
	int vertexIndex = index * 3;

	float x1 = vertexList_[vertexIndex].position.x;
	float z1 = vertexList_[vertexIndex].position.z;
	++vertexIndex;

	float x2 = vertexList_[vertexIndex].position.x;
	float z2 = vertexList_[vertexIndex].position.z;
	++vertexIndex;

	float x3 = vertexList_[vertexIndex].position.x;
	float z3 = vertexList_[vertexIndex].position.z;	

	// Check to see if the minimum of the x coordinates of the triangle is inside the node.
	float minimumX = min(x1, min(x2, x3));
	if(minimumX > (positionX + radius))
	{
		return false;
	}

	// Check to see if the maximum of the x coordinates of the triangle is inside the node.
	float maximumX = max(x1, max(x2, x3));
	if(maximumX < (positionX - radius))
	{
		return false;
	}

	// Check to see if the minimum of the z coordinates of the triangle is inside the node.
	float minimumZ = min(z1, min(z2, z3));
	if(minimumZ > (positionZ + radius))
	{
		return false;
	}

	// Check to see if the maximum of the z coordinates of the triangle is inside the node.
	float maximumZ = max(z1, max(z2, z3));
	if(maximumZ < (positionZ - radius))
	{
		return false;
	}

	return true;
}

/*
================
 Quadtree::releaseNode
================
*/
void Quadtree::releaseNode( QuadtreeNode* node)
{
	for (int i = 0; i < 4; ++i)
	{
		if (node->nodes[i] != 0)
		{
			releaseNode(node->nodes[i]);
		}
	}

	if (node->vertexBuffer)
	{
		node->vertexBuffer->Release();
		node->vertexBuffer = NULL;
	}

	if (node->indexBuffer)
	{
		node->indexBuffer->Release();
		node->indexBuffer = NULL;
	}

	if (node->vertexArray)
	{
		delete[] node->vertexArray;
		node->vertexArray = NULL;
	}

	for (int i = 0; i < 4; ++i)
	{
		if (node->nodes[i])
		{
			delete node->nodes[i];
			node->nodes[i] = NULL;
		}
	}

}

/*
================
 Quadtree::renderNode
================
*/
void Quadtree::renderNode( QuadtreeNode* node, Frustum* frustum, ID3D10Device* device, TerrainShader* shader)
{
	if(frustum->checkCube(node->positionX, 0.0f, node->positionZ, (node->width / 2.0f)))
	{
		int count = 0;
		
		for (int i = 0; i < 4; ++i)
		{		
			if (node->nodes[i] != NULL)
			{
				++count;
				renderNode(node->nodes[i], frustum, device, shader);
			}
		}

		if (count != 0)
		{
			return;
		}

		// Render this node
		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;

		device->IASetVertexBuffers(0, 1, &node->vertexBuffer, &stride, &offset);
		device->IASetIndexBuffer(node->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		int indexCount = node->triangleCount * 3;

		shader->renderShader(device, indexCount);

		drawCount_ += node->triangleCount;
	}
}

/*
================
 Quadtree::getHeightAtPosition
================
*/
bool Quadtree::getHeightAtPosition( float x, float z, float& height)
{
	float meshMinX = parentNode_->positionX - (parentNode_->width / 2.0f);
	float meshMaxX = parentNode_->positionX + (parentNode_->width / 2.0f);
	float meshMinZ = parentNode_->positionZ - (parentNode_->width / 2.0f);
	float meshMaxZ = parentNode_->positionZ + (parentNode_->width / 2.0f);

	if (x < meshMinX || x > meshMaxX || z < meshMinZ || z > meshMaxZ)
	{
		return false;
	}

	findNode(parentNode_, x, z, height);

	return true;	
}

/*
================
 Quadtree::findNode
================
*/
void Quadtree::findNode( QuadtreeNode* node, float x, float z, float& height )
{
	float xMin, xMax, zMin, zMax;
	int count, i, index;
	float vertex1[3], vertex2[3], vertex3[3];
	bool foundHeight;


	// Calculate the dimensions of this node.
	xMin = node->positionX - (node->width / 2.0f);
	xMax = node->positionX + (node->width / 2.0f);

	zMin = node->positionZ - (node->width / 2.0f);
	zMax = node->positionZ + (node->width / 2.0f);

	// See if the x and z coordinate are in this node, if not then stop traversing this part of the tree.
	if((x < xMin) || (x > xMax) || (z < zMin) || (z > zMax))
	{
		return;
	}

	// If the coordinates are in this node then check first to see if children nodes exist.
	count = 0;

	for(i=0; i<4; i++)
	{
		if(node->nodes[i] != 0)
		{
			count++;
			findNode(node->nodes[i], x, z, height);
		}
	}

	// If there were children nodes then return since the polygon will be in one of the children.
	if(count > 0)
	{
		return;
	}

	// If there were no children then the polygon must be in this node.  Check all the polygons in this node to find 
	// the height of which one the polygon we are looking for.
	for(i=0; i<node->triangleCount; i++)
	{
		index = i * 3;
		vertex1[0] = node->vertexArray[index].x;
		vertex1[1] = node->vertexArray[index].y;
		vertex1[2] = node->vertexArray[index].z;

		index++;
		vertex2[0] = node->vertexArray[index].x;
		vertex2[1] = node->vertexArray[index].y;
		vertex2[2] = node->vertexArray[index].z;

		index++;
		vertex3[0] = node->vertexArray[index].x;
		vertex3[1] = node->vertexArray[index].y;
		vertex3[2] = node->vertexArray[index].z;

		// Check to see if this is the polygon we are looking for.
		foundHeight = checkHeightOfTriangle(x, z, height, vertex1, vertex2, vertex3);

		// If this was the triangle then quit the function and the height will be returned to the calling function.
		if(foundHeight)
		{
			return;
		}
	}
}

/*
================
 Quadtree::checkHeightOfTriangle
================
*/
bool Quadtree::checkHeightOfTriangle( float x, float z, float& height, float v0[3], float v1[3], float v2[3] )
{
	float startVector[3], directionVector[3], edge1[3], edge2[3], normal[3];
	float Q[3], e1[3], e2[3], e3[3], edgeNormal[3], temp[3];
	float magnitude, D, denominator, numerator, t, determinant;


	// Starting position of the ray that is being cast.
	startVector[0] = x;
	startVector[1] = 0.0f;
	startVector[2] = z;

	// The direction the ray is being cast.
	directionVector[0] =  0.0f;
	directionVector[1] = -1.0f;
	directionVector[2] =  0.0f;

	// Calculate the two edges from the three points given.
	edge1[0] = v1[0] - v0[0];
	edge1[1] = v1[1] - v0[1];
	edge1[2] = v1[2] - v0[2];

	edge2[0] = v2[0] - v0[0];
	edge2[1] = v2[1] - v0[1];
	edge2[2] = v2[2] - v0[2];

	// Calculate the normal of the triangle from the two edges.
	normal[0] = (edge1[1] * edge2[2]) - (edge1[2] * edge2[1]);
	normal[1] = (edge1[2] * edge2[0]) - (edge1[0] * edge2[2]);
	normal[2] = (edge1[0] * edge2[1]) - (edge1[1] * edge2[0]);

	magnitude = (float)sqrt((normal[0] * normal[0]) + (normal[1] * normal[1]) + (normal[2] * normal[2]));
	normal[0] = normal[0] / magnitude;
	normal[1] = normal[1] / magnitude;
	normal[2] = normal[2] / magnitude;

	// Find the distance from the origin to the plane.
	D = ((-normal[0] * v0[0]) + (-normal[1] * v0[1]) + (-normal[2] * v0[2]));

	// Get the denominator of the equation.
	denominator = ((normal[0] * directionVector[0]) + (normal[1] * directionVector[1]) + (normal[2] * directionVector[2]));

	// Make sure the result doesn't get too close to zero to prevent divide by zero.
	if(fabs(denominator) < 0.0001f)
	{
		return false;
	}

	// Get the numerator of the equation.
	numerator = -1.0f * (((normal[0] * startVector[0]) + (normal[1] * startVector[1]) + (normal[2] * startVector[2])) + D);

	// Calculate where we intersect the triangle.
	t = numerator / denominator;

	// Find the intersection vector.
	Q[0] = startVector[0] + (directionVector[0] * t);
	Q[1] = startVector[1] + (directionVector[1] * t);
	Q[2] = startVector[2] + (directionVector[2] * t);

	// Find the three edges of the triangle.
	e1[0] = v1[0] - v0[0];
	e1[1] = v1[1] - v0[1];
	e1[2] = v1[2] - v0[2];

	e2[0] = v2[0] - v1[0];
	e2[1] = v2[1] - v1[1];
	e2[2] = v2[2] - v1[2];

	e3[0] = v0[0] - v2[0];
	e3[1] = v0[1] - v2[1];
	e3[2] = v0[2] - v2[2];

	// Calculate the normal for the first edge.
	edgeNormal[0] = (e1[1] * normal[2]) - (e1[2] * normal[1]);
	edgeNormal[1] = (e1[2] * normal[0]) - (e1[0] * normal[2]);
	edgeNormal[2] = (e1[0] * normal[1]) - (e1[1] * normal[0]);

	// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
	temp[0] = Q[0] - v0[0];
	temp[1] = Q[1] - v0[1];
	temp[2] = Q[2] - v0[2];

	determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));

	// Check if it is outside.
	if(determinant > 0.001f)
	{
		return false;
	}

	// Calculate the normal for the second edge.
	edgeNormal[0] = (e2[1] * normal[2]) - (e2[2] * normal[1]);
	edgeNormal[1] = (e2[2] * normal[0]) - (e2[0] * normal[2]);
	edgeNormal[2] = (e2[0] * normal[1]) - (e2[1] * normal[0]);

	// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
	temp[0] = Q[0] - v1[0];
	temp[1] = Q[1] - v1[1];
	temp[2] = Q[2] - v1[2];

	determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));

	// Check if it is outside.
	if(determinant > 0.001f)
	{
		return false;
	}

	// Calculate the normal for the third edge.
	edgeNormal[0] = (e3[1] * normal[2]) - (e3[2] * normal[1]);
	edgeNormal[1] = (e3[2] * normal[0]) - (e3[0] * normal[2]);
	edgeNormal[2] = (e3[0] * normal[1]) - (e3[1] * normal[0]);

	// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
	temp[0] = Q[0] - v2[0];
	temp[1] = Q[1] - v2[1];
	temp[2] = Q[2] - v2[2];

	determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));

	// Check if it is outside.
	if(determinant > 0.001f)
	{
		return false;
	}

	// Now we have our height.
	height = Q[1];

	return true;
}
