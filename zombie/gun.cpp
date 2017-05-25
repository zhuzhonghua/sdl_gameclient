#include "gun.h"

#include <time.h>
#include <random>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed, Zhu::SoundEffect fireEffect) :
_name(name),
_fireRate(fireRate),
_bulletsPerShot(bulletsPerShot),
_spread(spread),
_bulletDamage(bulletDamage),
_bulletSpeed(bulletSpeed),
_frameCount(0),
_fireEffect(fireEffect)
{

}

Gun::~Gun()
{

}

void Gun::update(bool isMouseDown, glm::vec2 position, glm::vec2 direction, std::vector<Bullet>& bullets, float deltaTime)
{
	_frameCount += 1.0f * deltaTime;
	if (_frameCount >= _fireRate && isMouseDown)
	{
		_frameCount = 0;
		fire(position, direction, bullets);
	}
}

void Gun::fire(glm::vec2 position, glm::vec2 direction, std::vector<Bullet>& bullets)
{
	static std::mt19937 randomEngine(time(NULL));
	std::uniform_real_distribution<float> randDir(-_spread, _spread);

	_fireEffect.play();

	for (int i = 0; i < _bulletsPerShot; i++)
	{
		float rndRot = randDir(randomEngine);
		glm::vec2 newDir = glm::rotate(direction, rndRot);
		bullets.emplace_back(position, newDir, _bulletDamage, _bulletSpeed);
	}
}