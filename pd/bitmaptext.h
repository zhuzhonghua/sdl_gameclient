#pragma once

#include <string>

#include "SDL_ttf.h"
#include "visual.h"
#include "smarttexture.h"

class BitmapText :public Visual{
public:
	BitmapText();
	BitmapText(const std::string& text);

	virtual void destroy();
	
	virtual void draw();

	std::string text() {return _text;}
	void text(const std::string& text);
	void measure();
protected:
	bool _dirty;
	TTF_Font* _font;
	std::string _text;
	std::vector<float> _vertices;

	SmartTexture* _texture;

	void updateVertices();
	virtual void updateMatrix();
};