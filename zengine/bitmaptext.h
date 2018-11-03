#pragma once

#include <string>

#include "SDL_ttf.h"
#include "gizmo.h"
#include "texture.h"
#include "glslprogram.h"

namespace Zengine{
class BitmapText :public Gizmo{
public:
	BitmapText();
	BitmapText(const std::string& text);
	BitmapText(Uint16 ch);

	void init();
	virtual void destroy();

	virtual void draw();

	std::string text() { return _text; }
	void text(const std::string& text);
	void measure();

	float baseLine();
	float lineHeight();
	float tracking();
protected:
	bool _dirty;
	static TTF_Font* _font;
	std::string _text;
	std::vector<GLSLProgram::Vertex> _vertices;
	int realLength;

	Texture* _texture;

	virtual void updateVertices();
	virtual void updateMatrix();
};
}