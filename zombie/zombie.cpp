#include "zombie.h"
#include "human.h"
#include "resourcemanager.h"

Zombie::Zombie()
{

}
Zombie::~Zombie()
{

}

void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zobies,
	float deltaTime)
{
	Human* closestHuman = getNearestHuman(humans);
	if (closestHuman != NULL)
	{
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += direction*_speed * deltaTime;
	}
	collideWithLevel(levelData);
}

void Zombie::init(float speed, glm::vec2 pos)
{
	_speed = speed;
	_position = pos;

	_color = Zhu::Color(255, 255, 255, 255);

	_health = 150;

	_texID = Zhu::ResourceManager::getTexture("data/zombie/Textures/zombie.png").id;
}

Human* Zombie::getNearestHuman(std::vector<Human*>& humans)
{
	Human* closestHuman = NULL;

	float smallestDist = 9999999999.0f;

	for (int i = 0; i < humans.size(); i++)
	{
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		float dist = glm::length(distVec);

		if (dist < smallestDist)
		{
			closestHuman = humans[i];
			smallestDist = dist;
		}
	}

	return closestHuman;
}