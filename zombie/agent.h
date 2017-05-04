#pragma once

#include "glm/glm.hpp"
#include "spritebatch.h"
#include "vertex.h"

const int AGENT_WIDTH = 60;

class Agent{
public:
	Agent();
	virtual ~Agent();

	virtual void update() = 0;

	void draw(Zhu::SpriteBatch& spriteBatch);

	glm::vec2 getPosition() const { return _position; }

protected:
	glm::vec2 _position;
	Zhu::Color _color;
	float _speed;
};