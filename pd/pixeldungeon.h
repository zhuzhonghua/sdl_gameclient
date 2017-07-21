#pragma once

#include "game.h"

class PixelDungeon :public Game{
public:
	PixelDungeon();
	~PixelDungeon();

	static bool landscape();
	static bool scaleUp();
};