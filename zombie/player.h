#pragma once

#include "human.h"

#include "bullet.h"

#include "inputmanager.h"
#include "camera2d.h"

class Gun;

class Player:public Human{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, Zhu::InputManager* inputMgr, Zhu::Camera2D* camera, std::vector<Bullet>* bullets);

	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zobies,
		float deltaTime);

	void addGun(Gun* gun);
private:

	Zhu::InputManager* _inputMgr;
	Zhu::Camera2D* _camera;
	std::vector<Bullet>* _bullets;

	std::vector<Gun*> _guns;
	int _currGunIdx;
};