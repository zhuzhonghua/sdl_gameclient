#pragma once

#include "glslprogram.h"
#include "gizmo.h"

namespace Zengine{
class Texture;
class Sprite:public Gizmo{
public:
	Sprite(const std::string& tx);
	void init();
	virtual ~Sprite();
	void draw();

	virtual void setPosition(float x, float y) { Gizmo::setPosition(x, y); updateVertex(); }
	virtual void setSize(float w, float h) { Gizmo::setSize(w, h); updateVertex(); }
protected:
	void updateVertex();

	Texture* _tex;
	std::vector<GLSLProgram::Vertex> _vertexData;
};
}