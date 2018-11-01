#pragma once

#include "glslprogram.h"

namespace Zengine{
class Texture;
class Image{
public:
	Image(const std::string& tx);
	void init();
	virtual ~Image();
	void draw();
protected:
	void updateVertex();
protected:
	Texture* _tex;
	std::vector<GLSLProgram::Vertex> _vertexData;
};
}