#pragma once

#include "glm/glm.hpp"
#include "spritebatch.h"
#include "vertex.h"

const int AGENT_WIDTH = 60;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Zombie;
class Human;

class Agent{
public:
	Agent();
	virtual ~Agent();

	virtual void update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zobies,
						float deltaTime) = 0;

	bool collideWithLevel(const std::vector<std::string>& levelData);

	bool collideWithAgent(Agent* agent);

	void draw(Zhu::SpriteBatch& spriteBatch);

	glm::vec2 getPosition() const { return _position; }

	bool applyDamage(float damage);
protected:
	void checkTilePosition(
		const std::vector<std::string>& levelData,
		std::vector<glm::vec2>& collideTilePosition,
		float x,
		float y);

	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 _position;
	Zhu::Color _color;
	float _speed;

	int _health;
};