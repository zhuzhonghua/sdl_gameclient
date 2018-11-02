#pragma once

#include "glslprogram.h"

namespace Zengine{
class Texture;
class Sprite{
public:
	Sprite(const std::string& tx);
	void init();
	virtual ~Sprite();
	void draw();

	float getWidth() { return _width; }
	float getHeight() { return _height; }
	float getPosX() { return _x; }
	float getPosY() { return _y; }

	void setPosition(float x, float y) { _x = x; _y = y; updateVertex(); }
	void setSize(float w, float h) { _width = w; _height = h; updateVertex(); }
protected:
	void updateVertex();
protected:
	float _width;
	float _height;
	float _x;
	float _y;
	Texture* _tex;
	std::vector<GLSLProgram::Vertex> _vertexData;
};
}