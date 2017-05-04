#pragma once

#include "glm/glm.hpp"
#include "spritebatch.h"

class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float speed, int lifeTime);
	~Bullet();

	void draw(Zhu::SpriteBatch& batch);
	bool update();

private:
	int _lifeTime;
	float _speed;
	
	glm::vec2 _position;
	glm::vec2 _direction;
};