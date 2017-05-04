#pragma once

#include "agent.h"
class Zombie:public Agent{
public:
	Zombie();
	~Zombie();

	void update();
};