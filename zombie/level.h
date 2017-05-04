#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "spritebatch.h"

const int TILE_WIDTH = 64;

class Level{
public:
	Level(std::string fileName);
	~Level();

	void draw();

	glm::vec2 getStartPlayerPos() const { return _startPlayerPos; }

	const std::vector<glm::vec2> const getZombieStartPos(){ return _zombieStartPosition; }

private:
	std::vector<std::string> _levelData;
	int _numHuman;

	Zhu::SpriteBatch _spriteBatch;
	glm::vec2	_startPlayerPos;
	std::vector<glm::vec2> _zombieStartPosition;
};