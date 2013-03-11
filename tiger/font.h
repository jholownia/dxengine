/*
================================
 font.h

 Main application object.

 Created on: 14 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <fstream>

#include "texture.h"
#include "common.h"

/*
================================
 
 Font

 A class for handling 2D font data.
 
================================
*/
class Font : Uncopyable
{
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Font(void);
	~Font(void);

	bool init(ID3D10Device*, char*, WCHAR*);
	void shutdown();

	ID3D10ShaderResourceView* getTexture();
	void buildVertexArray(void*, char*, float, float);

private:
	FontType* font_;
	Texture* texture_;
	
	bool loadFontData(char*);
	void releaseFontData();
	bool loadTexture(ID3D10Device*, WCHAR*);
	void releaseTexture();
};

