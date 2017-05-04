#include "spritebatch.h"
#include <algorithm>

using namespace Zhu;

SpriteBatch::SpriteBatch() :_vbo(0), _vao(0)
{

}
SpriteBatch::~SpriteBatch()
{

}

void SpriteBatch::init()
{
	createVertexArray();
}

void SpriteBatch::begin(GlyphSortType sortType/* = GlyphSortType::TEXTURE*/)
{
	_sortType = sortType;

	_renderBatches.clear();

	for (int i = 0; i < _glyphs.size(); i++)
	{
		delete _glyphs[i];
	}
	_glyphs.clear();
}

void SpriteBatch::end()
{
	sortGlyphs();
	createRenderBatches();
}
void SpriteBatch::draw(glm::vec4 destRect, glm::vec4 uvRect, GLuint tex, float depth,Color color)
{
	Glyph* newGlyph = new Glyph();
	newGlyph->tex = tex;
	newGlyph->depth = depth;

	newGlyph->topLeft.color = color;
	newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
	newGlyph->topLeft.setUV(uvRect.x, uvRect.y+uvRect.w);

	newGlyph->bottomLeft.color = color;
	newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
	newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

	newGlyph->bottomRight.color = color;
	newGlyph->bottomRight.setPosition(destRect.x+destRect.z, destRect.y);
	newGlyph->bottomRight.setUV(uvRect.x+uvRect.z, uvRect.y);

	newGlyph->topRight.color = color;
	newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
	newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

	_glyphs.push_back(newGlyph);
}

void SpriteBatch::renderBatch()
{
	glBindVertexArray(_vao);
	for (int i = 0; i < _renderBatches.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, _renderBatches[i].tex);
		glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset,_renderBatches[i].numVertices);
	}
	glBindVertexArray(0);
}

void SpriteBatch::createRenderBatches()
{
	std::vector<Vertex> vertices;
	vertices.resize(_glyphs.size()*6);

	if (_glyphs.size() <= 0)
	{
		return;
	}

	int cv = 0;
	int offset = 0;
	_renderBatches.emplace_back(offset, 6, _glyphs[0]->tex);
	vertices[cv++] = _glyphs[0]->topLeft;
	vertices[cv++] = _glyphs[0]->bottomLeft;
	vertices[cv++] = _glyphs[0]->bottomRight;
	vertices[cv++] = _glyphs[0]->bottomRight;
	vertices[cv++] = _glyphs[0]->topRight;
	vertices[cv++] = _glyphs[0]->topLeft;
	offset += 6;

	for (int cg = 1; cg < _glyphs.size(); cg++)
	{
		if (_glyphs[cg]->tex != _glyphs[cg - 1]->tex)
		{
			_renderBatches.emplace_back(offset, 6, _glyphs[cg]->tex);
		}
		else
		{
			_renderBatches.back().numVertices += 6;
		}
		vertices[cv++] = _glyphs[cg]->topLeft;
		vertices[cv++] = _glyphs[cg]->bottomLeft;
		vertices[cv++] = _glyphs[cg]->bottomRight;
		vertices[cv++] = _glyphs[cg]->bottomRight;
		vertices[cv++] = _glyphs[cg]->topRight;
		vertices[cv++] = _glyphs[cg]->topLeft;

		offset += 6;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(Vertex),vertices.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void SpriteBatch::createVertexArray()
{
	if (_vao == 0)
	{
		glGenVertexArrays(1, &(_vao));
	}
	glBindVertexArray(_vao);

	if (_vbo == 0)
	{
		glGenBuffers(1, &(_vbo));
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);
}

void SpriteBatch::sortGlyphs()
{
	switch (_sortType)
	{
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBackGlyphs);
		break;
	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFrontGlyphs);
		break;
	case GlyphSortType::TEXTURE:
		std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTextureGlyphs);
		break;
	}
}

bool SpriteBatch::compareFrontToBackGlyphs(Glyph* a, Glyph* b)
{
	return a->depth < b->depth;
}

bool SpriteBatch::compareBackToFrontGlyphs(Glyph* a, Glyph* b)
{
	return a->depth > b->depth;
}

bool SpriteBatch::compareTextureGlyphs(Glyph* a, Glyph* b)
{
	return a->tex < b->tex;
}