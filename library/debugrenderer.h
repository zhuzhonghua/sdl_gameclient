#pragma once

#include "vertex.h"
#include "glm/glm.hpp"
#include "glslprogram.h"
#include <vector>

namespace Zhu{
class DebugRenderer{
public:
	DebugRenderer();
	~DebugRenderer();

	void init();
	void end();
	void drawBox(const glm::vec4& destRect, const Color& color, float angle);
	void drawCircle(const glm::vec2& center, const Color& color, float radius);

	void render(const glm::mat4& mat, float lineWidth);
	void dispose();

	struct DebugVertex{
		glm::vec2 pos;
		Color color;
	};
private:
	GLSLProgram _program;
	std::vector<DebugVertex> _verts;
	std::vector<GLuint> _indices;
	GLuint _vbo, _vao, _ibo;

	int _numElements;
};
}