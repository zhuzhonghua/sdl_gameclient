#pragma once

#include "texture.h"
#include "spritebatch.h"
#include "Box2D/Box2D.h"

#include "box.h"
#include "glm/glm.hpp"
#include "inputmanager.h"

class Player
{
public:
	Player();
	~Player();

	void init(b2World* world, const glm::vec2& pos, const glm::vec2 dimen, Zhu::Color color, bool fixRotation);
	void draw(Zhu::SpriteBatch* sb);

	void update(Zhu::InputManager* inputMgr);

	Box& getBox() { return _box; }
private:
	Box _box;
};