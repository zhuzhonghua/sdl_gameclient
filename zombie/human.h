#pragma once

#include "agent.h"

class Human :public Agent{
public:
	Human();
	~Human();

	void update();
};