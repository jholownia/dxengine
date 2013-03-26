/*
================================
 debugwindow.h
 
 Created on: 20 Mar 2013
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2013.
================================
*/

#pragma once

#include <d3d10.h>
#include <d3dx10.h>

#include "common.h"

/*
================================
 
 DebugWindow

 A separate window used for rendering RenderTexture for debugging purposes.

================================
*/
class DebugWindow : Uncopyable
{
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
	
public:
	DebugWindow(void);
	~DebugWindow(void);

	bool init(ID3D10Device*, int, int, int, int);
	void shutdown();
	bool render(ID3D10Device*, int, int);

	int getIndexCount();

private:
	ID3D10Buffer* vertexBuffer_;
	ID3D10Buffer* indexBuffer_;
	int vertexCount_;
	int indexCount_;
	int screenWidth_;
	int screenHeight_;
	int bitmapWidth_;
	int bitmapHeight_;
	int previousPosX_;
	int previousPosY_;

	bool initBuffers(ID3D10Device*);
	void shutdownBuffers();
	bool updateBuffers(int, int);
	void renderBuffers(ID3D10Device*);
};

