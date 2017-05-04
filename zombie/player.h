#pragma once

#include "human.h"

#include "inputmanager.h"

class Player:public Human{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, Zhu::InputManager* inputMgr);

	void update();
private:

	Zhu::InputManager* _inputMgr;
};