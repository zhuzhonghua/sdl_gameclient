#include "stdafx.h"

#include "sprite.h"
#include "texture.h"

namespace Zengine{
Sprite::Sprite(const std::string& tx)
{
	init();
	_tex = new Texture(tx);
	_width = _tex->width;
	_height = _tex->height;
	_x = 0;
	_y = 0;

	updateVertex();
}

void Sprite::init()
{
	_tex = NULL;
	_vertexData.resize(4);
}

void Sprite::updateVertex()
{
	_vertexData[0].setPos(_x+_width, _y+_height);
	_vertexData[0].setUV(1, 0);

	_vertexData[1].setPos(_x, _y+_height);
	_vertexData[1].setUV(0, 0);

	_vertexData[2].setPos(_x, _y);
	_vertexData[2].setUV(0, 1);

	_vertexData[3].setPos(_x+_width, _y);
	_vertexData[3].setUV(1, 1);
}

Sprite::~Sprite()
{
	delete _tex;
}

void Sprite::draw()
{
	GLSLProgram::inst()->use();
	_tex->bind();
	GLSLProgram::Vertex* ptr = &_vertexData[0];
	GLSLProgram::inst()->drawQuad(ptr);
}
}