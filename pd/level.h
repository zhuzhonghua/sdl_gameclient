#pragma once

#include "bundlable.h"
#include <vector>

class Level :public Bundlable{
public:
	enum Feeling {
		NONE,
		CHASM,
		WATER,
		GRASS
	};

	static const int WIDTH = 32;
	static const int HEIGHT = 32;
	static const int LENGTH = WIDTH * HEIGHT;

	static std::vector<bool> fieldOfView;

	static std::vector<bool> passable;
	static std::vector<bool> losBlocking;
	static std::vector<bool> flamable;
	static std::vector<bool> secret;
	static std::vector<bool> solid;
	static std::vector<bool> avoid;
	static std::vector<bool> water;
	static std::vector<bool> pit;

	static std::vector<bool> discoverable;

	std::vector<int> map;
	std::vector<bool> visited;
	std::vector<bool> mapped;
};