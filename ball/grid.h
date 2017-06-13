#pragma once

#include "ball.h"
#include <vector>

struct Cell{
	std::vector<Ball*> balls;
};

class Grid{
	friend class BallController;
public:
	Grid(int width, int height, int cellSize);
	~Grid();

	void addBall(Ball* ball);
	void addBall(Ball* ball, Cell* cell);
	Cell* getCell(int x, int y);
	Cell* getCell(const glm::vec2& pos);

	void removeBallFromCell(Ball* ball);
private:
	std::vector<Cell> _cells;

	int _width;
	int _height;
	int _cellSize;

	int _numXCells;
	int _numYCells;
};