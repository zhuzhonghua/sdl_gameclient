#pragma once

#include "component.h"


const float Archs_SCROLL_SPEED = 20.0f;

class SkinnedBlock;

class Archs :public Component
{
private:
	SkinnedBlock* _arcsBg;
	SkinnedBlock* _arcsFg;

	static float _offsB;
	static float _offsF;

public:
	bool reversed;

	void update();

	Archs();
protected:
	void createChildren();
	void layout();
};