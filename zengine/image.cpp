#include "stdafx.h"

#include "image.h"
#include "texture.h"

namespace Zengine{
Image::Image(const std::string& tx)
{
	init();
	_tex = new Texture(tx);
}

void Image::init()
{
	_tex = NULL;
}

Image::~Image()
{
	delete _tex;
}

void Image::draw()
{
	_tex->bind();
	GLSLProgram::inst()->drawQuad();
}
}