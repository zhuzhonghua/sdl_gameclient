#pragma once
#include <string>
#include <vector>

#include "bullet.h"

#include "glm/glm.hpp"

class Gun
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed);
	~Gun();

	void update(bool isMouseDown, glm::vec2 position, glm::vec2 direction, std::vector<Bullet>& bullets, float deltaTime);

private:
	void fire(glm::vec2 position, glm::vec2 direction, std::vector<Bullet>& bullets);

	std::string _name;

	int _fireRate;
	float _frameCount;

	int _bulletsPerShot;
	float _spread;

	float _bulletSpeed;
	int _bulletDamage;
};