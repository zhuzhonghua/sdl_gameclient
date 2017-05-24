#include "bullet.h"
#include "resourcemanager.h"
#include "agent.h"
#include "human.h"
#include "zombie.h"
#include "level.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed)
{
	_position = position;
	_direction = direction;
	_speed = speed;
	_damage = damage;
}

Bullet::~Bullet()
{

}

void Bullet::draw(Zhu::SpriteBatch& batch)
{
	static Zhu::Texture tex = Zhu::ResourceManager::getTexture("data/zombie/Textures/circle.png");

	Zhu::Color color;
	color.r = 75;
	color.g = 75;
	color.b = 75;
	color.a = 255;

	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 position = glm::vec4(_position.x + BULLET_RADIUS, _position.y + BULLET_RADIUS, BULLET_RADIUS*2, BULLET_RADIUS*2);
	batch.draw(position, uv, tex.id, 0.0f, color);
}

bool Bullet::update(const std::vector<std::string>& levelData, float deltaTime)
{
	_position += _direction * _speed * deltaTime;
	return collideWithWorld(levelData);
}

bool Bullet::collideWithAgent(Agent* agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS +BULLET_RADIUS;

	glm::vec2 centerPosA = _position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData)
{
	glm::ivec2 gridPosition;
	gridPosition.x = floor(_position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(_position.y / (float)TILE_WIDTH);

	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].length() ||
		gridPosition.y < 0 || gridPosition.y >= levelData.size())
	{
		return true;
	}

	return (levelData[gridPosition.y][gridPosition.x] != '.');
}