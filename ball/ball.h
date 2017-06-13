#pragma once

#include "glm/glm.hpp"
#include "vertex.h"

class Cell;

struct Ball{
	Ball(float radius, float mass, const glm::vec2& pos,
			const glm::vec2& vel, unsigned int texId,
			const Zhu::Color& color);

	float radius;
	float mass;
	glm::vec2 velocity;
	glm::vec2 position;
	unsigned int texId;
	Zhu::Color color;
	Cell* ownerCell;
	int cellVectorIndex;
};