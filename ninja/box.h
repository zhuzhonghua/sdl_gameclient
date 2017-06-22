#pragma once

#include "Box2D/Box2D.h"
#include "glm/glm.hpp"
#include "vertex.h"
#include "spritebatch.h"
#include "texture.h"

class Box
{
public:
	Box();
	~Box();

	void init(b2World* world, const glm::vec2& pos, const glm::vec2 dimen, Zhu::Texture tex, Zhu::Color color, bool fixRotation, glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	void draw(Zhu::SpriteBatch* sb);

	b2Body* getBody() const { return _body; }
	b2Fixture* getFixture() const { return _fixture; }
	glm::vec2 getDimen() const { return _dimen; }
	Zhu::Color getColor() const { return _color; }
private:
	b2Body* _body;
	b2Fixture* _fixture;

	glm::vec2 _dimen;
	Zhu::Color _color;

	Zhu::Texture _tex;
	glm::vec4 _uvRect;
};