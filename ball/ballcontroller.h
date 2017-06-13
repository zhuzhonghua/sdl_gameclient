#pragma once

#include <vector>

#include "ball.h"

enum GravityDirection { NONE, LEFT, UP, RIGHT, DOWN};

class Grid;

class BallController{
public:
	BallController();

	void updateBalls(std::vector<Ball>& balls, Grid* grid, float deltaTime, int maxX, int maxY);

	void onMouseDown(std::vector<Ball>& balls, float mouseX, float mouseY);
	void onMouseUp(std::vector<Ball>& balls);
	void onMouseMove(std::vector<Ball>& balls, float mouseX, float mouseY);

	void setGravityDirection(GravityDirection dir) { _gravityDir = dir; }

	void updateCollision(Grid* grid);
private:
	void checkCollision(Ball* b, std::vector<Ball*>& balls, int startIndex);
	void checkCollision(Ball& b1, Ball& b2);
	bool isMouseOnBall(Ball& b, float mouseX, float mouseY);
	glm::vec2 getGravityAccel();

	int _grabbedBall;
	glm::vec2 _prePos;
	glm::vec2 _gradOffset;
	GravityDirection _gravityDir;
};