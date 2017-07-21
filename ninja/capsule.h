#pragma once

#include "Box2D/Box2D.h"
#include "glm/glm.hpp"
#include "debugrenderer.h"

class Capsule{
public:
	void init(b2World* world, const glm::vec2& pos, const glm::vec2 dimen, float density, float friction, bool fixRotation);
	void drawDebug(Zhu::DebugRenderer* debugRenderer);

	b2Body* getBody() const { return _body; }
	glm::vec2 getDimen() const { return _dimen; }
	b2Fixture* getFixture(int idx) const { return _fixtures[idx]; }
private:
	b2Body* _body;
	b2Fixture* _fixtures[3];
	glm::vec2 _dimen;
};