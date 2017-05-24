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
	Glyph(glm::vec4 destRect, glm::vec4 uvRect, GLuint Tex, float Depth, Color color)
	{
		tex = Tex;
		depth = Depth;

		topLeft.color = color;
		topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x, destRect.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}
	GLuint tex;
	float depth;

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
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