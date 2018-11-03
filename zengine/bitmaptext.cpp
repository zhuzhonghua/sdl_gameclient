#include "stdafx.h"
#include "bitmaptext.h"

#include "glslprogram.h"

namespace Zengine{
TTF_Font* BitmapText::_font = NULL;

void BitmapText::init()
{
	_vertices.resize(4);
	_texture = NULL;
	_dirty = false;
}

BitmapText::BitmapText()
{
	init();
}

BitmapText::BitmapText(const std::string& t)
{
	init();

	if (_font == NULL)
	{
		_font = TTF_OpenFont("data/msyhbd.ttf", 32);
		if (!_font) fatalError("fail open font");
		//TTF_SetFontKerning(_font, 2);
	}

	text(t);
}

BitmapText::BitmapText(Uint16 ch)
{
	init();

	if (_font == NULL)
	{
		_font = TTF_OpenFont("data/msyhbd.ttf", 32);
		if (!_font) fatalError("fail open font");
		//TTF_SetFontKerning(_font, 4);
	}

	const SDL_Color c = { 255, 255, 255 };

	SDL_Surface* surface = TTF_RenderGlyph_Blended(_font, ch, c);
	_texture = new Texture(surface);
}

void BitmapText::destroy()
{
}

void BitmapText::updateMatrix()
{
}

void BitmapText::draw()
{
	GLSLProgram* script = GLSLProgram::inst();

	if (_texture)
	{
		if (_dirty)
		{
			updateVertices();
			_dirty = false;
		}
		GLSLProgram::inst()->use();
		_texture->bind();

		GLSLProgram::Vertex* ptr = &_vertices[0];
		GLSLProgram::inst()->drawQuad(ptr);
	}
}

void BitmapText::updateVertices()
{
	int w = 0, h = 0;
	TTF_SizeUTF8(_font, _text.c_str(), &w, &h);

	_width = w;
	_height = h;

	_vertices[0].setPos(_x + _width, _y + _height);
	_vertices[0].setUV(1, 0);

	_vertices[1].setPos(_x, _y + _height);
	_vertices[1].setUV(0, 0);

	_vertices[2].setPos(_x, _y);
	_vertices[2].setUV(0, 1);

	_vertices[3].setPos(_x + _width, _y);
	_vertices[3].setUV(1, 1);
}

void BitmapText::text(const std::string& str)
{
	if (str.length() <= 0) return;

	const SDL_Color c = { 255, 255, 255 };

	_text = str;

	// TODO: Rewrite,split line to single word
	// dynamic input string
	SDL_Surface* surface = TTF_RenderUTF8_Blended(_font, str.c_str(), c);

	if (_texture == NULL)
	{
		_texture = new Texture(surface);
	}
	else
	{
		_texture->Texture::bitmap(surface);
	}

	_dirty = true;
}

void BitmapText::measure()
{
	int w = 0, h = 0;
	TTF_SizeUTF8(_font, _text.c_str(), &w, &h);

	_width = w;
	_height = h;
}

float BitmapText::baseLine()
{
	return 32;
}

float BitmapText::lineHeight()
{
	return 32;
}

float BitmapText::tracking()
{
	return -1;
}

}