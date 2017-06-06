#include "human.h"
#include "resourcemanager.h"
#include <time.h>
#include <random>
#include <glm/gtx/rotate_vector.hpp>

Human::Human()
{
	_frames = 0;
}
Human::~Human()
{

}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zobies,
	float deltaTime)
{
	static std::mt19937 randomEngine(time(NULL));
	static std::uniform_real_distribution<float> randRotate(-10.0f, 10.0f);


	_position += _direction * _speed * deltaTime;

	if (_frames == 20)
	{
		_direction = glm::rotate(_direction, randRotate(randomEngine));
		_frames = 0;
	}
	else
	{
		_frames = 0;
	}
	

	if (collideWithLevel(levelData))
	{
		_direction = glm::rotate(_direction, randRotate(randomEngine));
	}
}

void Human::init(float speed, glm::vec2 pos)
{
	static std::mt19937 randomEngine(time(NULL));	
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;

	_speed = speed;
	_position = pos;

	_health = 10;

	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	if (_direction.length() == 0) _direction = glm::vec2(1.0f, 0.0f);

	_direction = glm::normalize(_direction);

	_texID = Zhu::ResourceManager::getTexture("data/zombie/Textures/human.png").id;
}