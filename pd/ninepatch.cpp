#include "ninepatch.h"
#include "texturecache.h"
#include "quad.h"
#include "util.h"
#include "noosascript.h"

NinePatch::NinePatch(const std::string& tx, int margin) 
:NinePatch(tx, margin, margin, margin, margin)
{

}

NinePatch::NinePatch(const std::string& tx, int left, int top, int right, int bottom) 
:NinePatch(tx, 0, 0, 0, 0, left, top, right, bottom)
{

}

NinePatch::NinePatch(const std::string& tx, int x, int y, int w, int h, int margin)
: NinePatch(tx, x, y, w, h, margin, margin, margin, margin)
{

}

NinePatch::NinePatch(const std::string& tx, int x, int y, int w, int h, int left, int top, int right, int bottom)
:Visual(0, 0, 0, 0)
{
	texture = TextureCache::getTexture(tx);
	w = w == 0 ? texture->width : w;
	h = h == 0 ? texture->height : h;

	_nWidth = widthf = w;
	_nHeight = heightf = h;

	_vertices.resize(16*9);

	_marginLeft = left;
	_marginRight = right;
	_marginTop = top;
	_marginBottom = bottom;

	_outterF = texture->uvRect(x, y, x + w, y + h);
	_innerF = texture->uvRect(x + left, y + top, x + w - right, y + h - bottom);

	updateVertices();
}

void NinePatch::updateVertices()
{
	float right = widthf - _marginRight;
	float bottom = heightf - _marginBottom;

	Quad::fill(&_vertices[16 * 0], 0, 
		_marginLeft, 0, _marginTop, GameMath::RECTFLeft(_outterF), 
		GameMath::RECTFLeft(_innerF), GameMath::RECTFTop(_outterF), GameMath::RECTFTop(_innerF));

	Quad::fill(&_vertices[16 * 1], 
		_marginLeft, right, 0, _marginTop, 
		GameMath::RECTFLeft(_innerF), GameMath::RECTFRight(_innerF), GameMath::RECTFTop(_outterF), GameMath::RECTFTop(_innerF));

	Quad::fill(&_vertices[16 * 2], 
		right, widthf, 0, _marginTop, GameMath::RECTFRight(_innerF), GameMath::RECTFRight(_outterF),
		GameMath::RECTFTop(_outterF), GameMath::RECTFTop(_innerF));

	Quad::fill(&_vertices[16 * 3], 0, 
		_marginLeft, _marginTop, bottom, GameMath::RECTFLeft(_outterF), 
		GameMath::RECTFLeft(_innerF), GameMath::RECTFTop(_innerF), GameMath::RECTFBottom(_innerF));

	Quad::fill(&_vertices[16 * 4], _marginLeft, right, 
		_marginTop, bottom, GameMath::RECTFLeft(_innerF), GameMath::RECTFRight(_innerF),
		GameMath::RECTFTop(_innerF), GameMath::RECTFBottom(_innerF));

	Quad::fill(&_vertices[16 * 5], right, widthf, _marginTop, bottom, GameMath::RECTFRight(_innerF), GameMath::RECTFRight(_outterF),
		GameMath::RECTFTop(_innerF), GameMath::RECTFBottom(_innerF));

	Quad::fill(&_vertices[16 * 6], 0, _marginLeft, bottom, heightf, 
		GameMath::RECTFLeft(_outterF), GameMath::RECTFLeft(_innerF), GameMath::RECTFBottom(_innerF), GameMath::RECTFBottom(_outterF));

	Quad::fill(&_vertices[16 * 7], _marginLeft, right, bottom, heightf, 
		GameMath::RECTFLeft(_innerF), GameMath::RECTFRight(_innerF), GameMath::RECTFBottom(_innerF), GameMath::RECTFBottom(_outterF));

	Quad::fill(&_vertices[16 * 8], right, widthf, bottom, heightf, 
		GameMath::RECTFRight(_innerF), GameMath::RECTFRight(_outterF), GameMath::RECTFBottom(_innerF), GameMath::RECTFBottom(_outterF));
}

void NinePatch::size(float width, float height)
{
	this->widthf = width;
	this->heightf = height;
	updateVertices();
}

void NinePatch::draw()
{
	Visual::draw();

	NoosaScript* script = NoosaScript::get();

	texture->bind();

	script->camera(camera());

	script->uModel.valueM4(_mat);
	script->lighting(
		rm, gm, bm, am,
		ra, ga, ba, aa);

	script->drawQuadSet(&_vertices[0], 9);
}