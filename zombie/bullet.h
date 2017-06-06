#pragma once

#include "glm/glm.hpp"
#include "spritebatch.h"

class Human;
class Zombie;
class Agent;

const int BULLET_RADIUS = 5;

class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Bullet();

	void draw(Zhu::SpriteBatch& batch);
	bool update(const std::vector<std::string>& levelData, float deltaTime);

	bool collideWithAgent(Agent* agent);

	float getDamage() const { return _damage; }
	glm::vec2 getPosition() const { return _position; }
private:
	bool collideWithWorld(const std::vector<std::string>& levelData);

	float _damage;

	float _speed;
	
	glm::vec2 _position;
	glm::vec2 _direction;
};