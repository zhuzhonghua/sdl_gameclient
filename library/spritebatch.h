#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "vertex.h"

#include <vector>

namespace Zhu{

enum GlyphSortType{
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE
};

struct Glyph
{
	Glyph(){}
	Glyph(glm::vec4 destRect, glm::vec4 uvRect, GLuint Tex, float Depth, Color color);
	Glyph(glm::vec4 destRect, glm::vec4 uvRect, GLuint Tex, float Depth, Color color, float angle);

	GLuint tex;
	float depth;

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;

private:
	glm::vec2 rotatePoint(glm::vec2 pos, float angle);
};

class RenderBatch{
public:
	RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Tex) :offset(Offset),
		numVertices(NumVertices), tex(Tex){}
	GLuint offset;
	GLuint numVertices;
	GLuint tex;
};
class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	void init();

	void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
	void end();
	void draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint tex, float depth, Color color);
	void draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint tex, float depth, Color color, float angle);
	void draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint tex, float depth, Color color, const glm::vec2& dir);
	void renderBatch();

private:
	void createVertexArray();
	void createRenderBatches();
	void sortGlyphs();

	static bool compareFrontToBackGlyphs(Glyph* a, Glyph* b);
	static bool compareBackToFrontGlyphs(Glyph* a, Glyph* b);
	static bool compareTextureGlyphs(Glyph* a, Glyph* b);

	GLuint _vbo;
	GLuint _vao;

	GlyphSortType _sortType;

	std::vector<Glyph*>  _glyphPointers;
	std::vector<Glyph>  _glyphs;
	std::vector<RenderBatch> _renderBatches;
};
};