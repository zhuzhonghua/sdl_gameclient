#include "ball.h"

Ball::Ball(float radius, float mass, const glm::vec2& pos,
	const glm::vec2& vel, unsigned int texId,
	const Zhu::Color& color)
{
	this->radius = radius;
	this->mass = mass;
	this->velocity = vel;
	this->position = pos;
	this->texId = texId;
	this->color = color;

	ownerCell = NULL;

	cellVectorIndex = -1;
}