/*
================================
 text.cpp

 Created on: 17 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include "font.h"
#include "fontshader.h"
/*
================================
 
 Text

 A 2D text object used for rendering text to screen.
 
================================
*/
class Text
{
public:
	Text(void);
	~Text(void);

	bool init(ID3D10Device*, HWND, int, int, D3DXMATRIX);
	void shutdown();
	void render(ID3D10Device*, D3DXMATRIX, D3DXMATRIX);

	bool setMousePosition(int, int);
	bool setFps(int);
	bool setCpu(int);
	bool setPolyCount(int);

private:
	struct SentenceType
	{
		ID3D10Buffer* vertexBuffer;
		ID3D10Buffer* indexBuffer;
		int vertexCount;
		int indexCount;
		int maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
	
	Font* font_;
	FontShader* fontShader_;
	int screenWidth_;
	int screenHeight_;
	D3DXMATRIX baseViewMatrix_;
	SentenceType* sentence1_;
	SentenceType* sentence2_;
	SentenceType* sentence3_;

	bool initSentence(SentenceType**, int, ID3D10Device*);
	bool updateSentence(SentenceType*, char*, int, int, float, float, float);
	void releaseSentence(SentenceType**);
	void renderSentence(ID3D10Device*, SentenceType*, D3DXMATRIX, D3DXMATRIX);
};

