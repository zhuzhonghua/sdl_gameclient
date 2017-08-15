#pragma once

#include "define.h"

class DungeonTilemap{
public:
	static const int SIZE = 16;
	static PointF tileToWorld(int pos);
};