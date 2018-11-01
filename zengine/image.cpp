#include "stdafx.h"

#include "image.h"
#include "texture.h"

namespace Zengine{
Image::Image(const std::string& tx)
{
	init();
	_tex = new Texture(tx);
	updateVertex();
}

void Image::init()
{
	_tex = NULL;
	_vertexData.resize(4);
}

void Image::updateVertex()
{
	_vertexData[0].setPos(0, 0);
	_vertexData[0].setUV(1, 0);

	_vertexData[1].setPos(-1, 0);
	_vertexData[1].setUV(0, 0);

	_vertexData[2].setPos(-1, -1);
	_vertexData[2].setUV(0, 1);

	_vertexData[3].setPos(0, -1);
	_vertexData[3].setUV(1, 1);
}

Image::~Image()
{
	delete _tex;
}

void Image::draw()
{
	GLSLProgram::inst()->use();
	_tex->bind();
	GLSLProgram::Vertex* ptr = &_vertexData[0];
	GLSLProgram::inst()->drawQuad(ptr);
}
}