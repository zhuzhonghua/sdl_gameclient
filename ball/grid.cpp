#include "grid.h"
#include "ball.h"

Grid::Grid(int width, int height, int cellSize)
{
	_width = width;
	_height = height;
	_cellSize = cellSize;

	_numXCells = ceil((float)_width/_cellSize);
	_numYCells = ceil((float)_height / _cellSize);

	_cells.resize(_numYCells*_numXCells);
}

Grid::~Grid()
{

}

Cell* Grid::getCell(int x, int y)
{
	if (x < 0) x = 0;
	if (x >= _numXCells) x = _numXCells-1;

	if (y < 0) y = 0;
	if (y >= _numYCells) y = _numYCells-1;

	return &_cells[y*_numXCells+x];
}

Cell* Grid::getCell(const glm::vec2& pos)
{
	int cellX = pos.x / _cellSize;
	int cellY = pos.y / _cellSize;

	return getCell(cellX, cellY);
}

void Grid::addBall(Ball* ball)
{
	Cell* cell = getCell(ball->position);
	cell->balls.push_back(ball);
	ball->ownerCell = cell;
	ball->cellVectorIndex = cell->balls.size() - 1;
}

void Grid::addBall(Ball* ball, Cell* cell)
{
	cell->balls.push_back(ball);
	ball->ownerCell = cell;
	ball->cellVectorIndex = cell->balls.size() - 1;
}

void Grid::removeBallFromCell(Ball* ball)
{
	std::vector<Ball*>& balls = ball->ownerCell->balls;
	balls[ball->cellVectorIndex] = balls.back();
	balls.pop_back();

	if (ball->cellVectorIndex < balls.size()){
		balls[ball->cellVectorIndex]->cellVectorIndex = ball->cellVectorIndex;
	}

	ball->ownerCell = NULL;
	ball->cellVectorIndex = -1;
}