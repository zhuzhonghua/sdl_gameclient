#pragma once

#include "agent.h"

class Human :public Agent{
public:
	Human();
	~Human();

	void init(float speed, glm::vec2 pos);
	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zobies,
		float deltaTime);

private:
	int _frames;
};