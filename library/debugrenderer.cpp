#include "debugrenderer.h"

using namespace Zhu;

namespace{
	const char* VERT_SRC = R"(#version 130
//The vertex shader operates on each vertex

//input data from the VBO. Each vertex is 2 floats
in vec2 vertexPosition;
in vec4 vertexColor;

out vec2 fragmentPosition;
out vec4 fragmentColor;

uniform mat4 P;

void main() {
    //Set the x,y position on the screen
    gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
    //the z position is zero since we are in 2D
    gl_Position.z = 0.0;
    
    //Indicate that the coordinates are normalized
    gl_Position.w = 1.0;
    
    fragmentPosition = vertexPosition;
    
    fragmentColor = vertexColor;
})";

	const char* FRAG_SRC = R"(#version 130
//The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;

//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
out vec4 color;

void main() {

    color = fragmentColor;
})";
}

DebugRenderer::DebugRenderer()
{
	_vao = 0;
	_vbo = 0;
	_ibo = 0;
	_numElements = 0;
}

DebugRenderer::~DebugRenderer()
{

}

void DebugRenderer::init()
{
	_program.compileShadersFromSource(VERT_SRC, FRAG_SRC);
	_program.addAttribute("vertexPosition");
	_program.addAttribute("vertexColor");
	_program.linkShaders();

	glGenVertexArrays(1,&_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, pos));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

	glBindVertexArray(0);
}

void DebugRenderer::end()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _verts.size()*sizeof(DebugVertex), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, _verts.size()*sizeof(DebugVertex), _verts.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size()*sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _indices.size()*sizeof(GLuint), _indices.data());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	_numElements = _indices.size();
	_indices.clear();
	_verts.clear();
}

namespace{
	glm::vec2 rotatePoint(glm::vec2 pos, float angle)
	{
		glm::vec2 newv;
		newv.x = pos.x*cos(angle) - pos.y * sin(angle);
		newv.y = pos.x*sin(angle) + pos.y * cos(angle);

		return newv;
	}
}

void DebugRenderer::drawBox(const glm::vec4& destRect, const Color& color, float angle)
{
	glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

	glm::vec2 tl(-halfDims.x, halfDims.y);
	glm::vec2 bl(-halfDims.x, -halfDims.y);
	glm::vec2 br(halfDims.x, -halfDims.y);
	glm::vec2 tr(halfDims.x, halfDims.y);

	tl = rotatePoint(tl, angle) + halfDims;
	bl = rotatePoint(bl, angle) + halfDims;
	br = rotatePoint(br, angle) + halfDims;
	tr = rotatePoint(tr, angle) + halfDims;

	glm::vec2 posOffset(destRect.x, destRect.y);

	int i = _verts.size();
	_verts.resize(i + 4);
	_verts[i].pos = tl + posOffset;
	_verts[i + 1].pos = bl + posOffset;
	_verts[i + 2].pos = br + posOffset;
	_verts[i + 3].pos = tr + posOffset;

	for (int j = i; j < _verts.size(); j++)
	{
		_verts[j].color = color;
	}

	_indices.reserve(_indices.size() + 8);
	_indices.push_back(i);
	_indices.push_back(i+1);

	_indices.push_back(i+1);
	_indices.push_back(i + 2);

	_indices.push_back(i + 2);
	_indices.push_back(i + 3);

	_indices.push_back(i + 3);
	_indices.push_back(i);
}

void DebugRenderer::drawCircle(const glm::vec2& center, const Color& color, float radius)
{
	const int NUM_VERTS = 100;
	int start = _verts.size();
	_verts.resize(start+NUM_VERTS);

	for (int i = 0; i < NUM_VERTS; i++)
	{
		float angle = ((float)i / NUM_VERTS) * 2 * M_PI;
		_verts[start + i].pos.x = center.x + cos(angle)*radius;
		_verts[start + i].pos.y = center.y + sin(angle)*radius;
		_verts[start + i].color = color;
	}

	_indices.reserve(_indices.size() + 2 * NUM_VERTS);
	for (int i = 0; i < NUM_VERTS - 1; i++)
	{
		_indices.push_back(start + i);
		_indices.push_back(start + i+1);
	}
	_indices.push_back(start + NUM_VERTS - 1);
	_indices.push_back(start);
}

void DebugRenderer::render(const glm::mat4& mat, float lineWidth)
{
	_program.use();

	GLuint pLocation = _program.getUniformLocation("P");
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(mat[0][0]));

	glLineWidth(lineWidth);
	glBindVertexArray(_vao);
	glDrawElements(GL_LINES, _numElements, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	_program.unuse();
}

void DebugRenderer::dispose()
{
	if (_vao) glDeleteVertexArrays(1, &_vao);
	if (_vbo) glDeleteBuffers(1, &_vbo);
	if (_ibo) glDeleteBuffers(1, &_ibo);

	_program.dispose();
}