#pragma once

#include "spritebatch.h"
#include "vertex.h"
#include "glm/glm.hpp"

class Light{
public:
	void draw(Zhu::SpriteBatch* batch)
	{
		glm::vec4 destRect;
		destRect.x = pos.x - size / 2;
		destRect.y = pos.y - size / 2;
		destRect.z = size;
		destRect.w = size;

		batch->draw(destRect, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0, color, 0.0f);
	}

	Zhu::Color color;
	glm::vec2 pos;
	float size;
};