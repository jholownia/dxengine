/*
================================
 font.cpp

 Main application object.

 Created on: 14 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "font.h"

/*
================
 Font::Font
================
*/
Font::Font(void) :
	font_    (NULL),
	texture_ (NULL)
{

}

/*
================
 Font::~Font
================
*/
Font::~Font(void)
{

}

/*
================
 Font::init

 Loads font data and font texture.
================
*/
bool Font::init( ID3D10Device* device, char* fontFilename, WCHAR* textureFilename)
{
	bool result;

	result = loadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	result = loadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

/*
================
 Font::shutdown
================
*/
void Font::shutdown()
{
	releaseTexture();
	releaseFontData();
}

/*
================
 Font::getTexture
================
*/
ID3D10ShaderResourceView* Font::getTexture()
{
	return texture_->getTexture();
}

/*
================
 Font::buildVertexArray

 Creates vertex buffers for rendering text sentences.
================
*/
void Font::buildVertexArray( void* vertices, char* sentence, float x, float y)
{
	VertexType* vertexPtr;	
	int letter;
	int index = 0;

	vertexPtr = (VertexType *) vertices;

	int numLetters = (int) strlen(sentence);

	// Draw each letter onto a quad
	for (int i = 0; i < numLetters; ++i)
	{
		letter = ((int) sentence[i]) - 32;

		// If the letter is a space then just move over three pixels
		if (letter == 0)
		{
			x = x + 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(x, y, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(font_[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((x + font_[letter].size), (y - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(font_[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(x, (y - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = D3DXVECTOR2(font_[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(x, y, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(font_[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(x + font_[letter].size, y, 0.0f);  // Top right.
			vertexPtr[index].texture = D3DXVECTOR2(font_[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((x + font_[letter].size), (y - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(font_[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			x = x + font_[letter].size + 1.0f;
		}
	}
}

/*
================
 Font::loadFontData
================
*/
bool Font::loadFontData( char* filename)
{
	std::ifstream ifs;
	char input;
	font_ = new FontType[95];

	ifs.open(filename);
	if (ifs.fail())
	{
		return false;
	}

	for (int i = 0; i < 95; ++i)
	{
		ifs.get(input);
		while (input != ' ')
		{
			ifs.get(input);
		}
		ifs.get(input);
		while (input != ' ')
		{
			ifs.get(input);
		}

		ifs >> font_[i].left;
		ifs >> font_[i].right;
		ifs >> font_[i].size;
	}

	ifs.close();

	return true;
}

/*
================
 Font::releaseFontData
================
*/
void Font::releaseFontData()
{
	if (font_)
	{
		delete[] font_;
		font_ = NULL;
	}
}

/*
================
 Font::loadTexture
================
*/
bool Font::loadTexture( ID3D10Device* device, WCHAR* filename)
{
	texture_ = new Texture;

	bool result = texture_->init(device, filename);

	return result;
}

/*
================
 Font::releaseTexture
================
*/
void Font::releaseTexture()
{
	if (texture_)
	{
		texture_->shutdown();
		delete texture_;
		texture_ = NULL;
	}
}
