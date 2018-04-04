#include "ballcontroller.h"
#include "grid.h"

BallController::BallController()
{
	_grabbedBall = -1;
	_prePos = glm::vec2(0.0f);
	_gradOffset = glm::vec2(0.0f);
	_gravityDir = GravityDirection::NONE;
}

void BallController::updateBalls(std::vector<Ball>& balls, Grid* grid, float deltaTime, int maxX, int maxY)
{
	const float FRICTION = 0.001f;
	if (_grabbedBall != -1)
	{
		balls[_grabbedBall].velocity = balls[_grabbedBall].position - _prePos;
	}

	glm::vec2 gravity = getGravityAccel();

	for (int i = 0; i < balls.size(); i++)
	{
		Ball& ball = balls[i];
		if (i != _grabbedBall)
		{
			ball.position += ball.velocity * deltaTime;

			glm::vec2 momentumVec = ball.velocity * ball.mass;
			if (momentumVec.x != 0 || momentumVec.y != 0)
			{
				if (FRICTION < glm::length(momentumVec))
				{
					ball.velocity -= deltaTime * FRICTION * glm::normalize(momentumVec) / ball.mass;
				}
				else
				{
					ball.velocity = glm::vec2(0.0f);
				}
			}

			ball.velocity += gravity * deltaTime;
		}

		if (ball.position.x < ball.radius)
		{
			ball.position.x = ball.radius;
			if (ball.velocity.x < 0)
			{
				ball.velocity.x *= -1;
			}
		}
		else if (ball.position.x + ball.radius>= maxX)
		{
			ball.position.x = maxX - ball.radius - 1;
			if (ball.velocity.x > 0)
			{
				ball.velocity.x *= -1;
			}
		}

		if (ball.position.y < ball.radius)
		{
			ball.position.y = ball.radius;
			if (ball.velocity.y < 0)
			{
				ball.velocity.y *= -1;
			}
		}
		else if (ball.position.y + ball.radius >= maxY)
		{
			ball.position.y = maxY - ball.radius - 1;
			if (ball.velocity.y > 0)
			{
				ball.velocity.y *= -1;
			}
		}

		Cell* newCell = grid->getCell(ball.position);
		if (newCell != ball.ownerCell)
		{
			grid->removeBallFromCell(&balls[i]);
			grid->addBall(&balls[i], newCell);
		}
	}

	updateCollision(grid);

	if (_grabbedBall != -1)
	{
		balls[_grabbedBall].velocity = balls[_grabbedBall].position - _prePos;
		_prePos = balls[_grabbedBall].position;
	}
}

void BallController::onMouseDown(std::vector<Ball>& balls, float mouseX, float mouseY)
{
	for (int i = 0; i < balls.size(); i++)
	{
		if (isMouseOnBall(balls[i], mouseX, mouseY))
		{
			_grabbedBall = i;
			_gradOffset = glm::vec2(mouseX, mouseY) - balls[i].position;
			_prePos = balls[i].position;
			balls[i].velocity = glm::vec2(0.0f);

			break;
		}
	}
}

void BallController::onMouseUp(std::vector<Ball>& balls)
{
	if (_grabbedBall != -1)
	{
		balls[_grabbedBall].velocity = balls[_grabbedBall].position - _prePos;
		_grabbedBall = -1;
	}
}

void BallController::onMouseMove(std::vector<Ball>& balls, float mouseX, float mouseY)
{
	if (_grabbedBall != -1)
	{
		balls[_grabbedBall].position = glm::vec2(mouseX, mouseY) - _gradOffset;
	}
}

void BallController::updateCollision(Grid* grid)
{
	for (int i = 0; i < grid->_cells.size(); i++)
	{
		int x = i % grid->_numXCells;
		int y = i / grid->_numXCells;

		Cell* cell = grid->getCell(x, y);
		for (int j = 0; j < cell->balls.size(); j++)
		{
			Ball* ball = cell->balls[j];
			checkCollision(ball, cell->balls, j+1);

			if (x > 0)
			{
				checkCollision(ball, grid->getCell(x - 1, y)->balls, 0);
				if (y > 0)
				{
					checkCollision(ball, grid->getCell(x-1,y-1)->balls, 0);
				}
				if (y < grid->_numYCells - 1)
				{
					checkCollision(ball, grid->getCell(x - 1, y + 1)->balls, 0);
				}
			}

			if (y > 0)
			{
				checkCollision(ball, grid->getCell(x, y - 1)->balls, 0);
			}
		}
	}
}

void BallController::checkCollision(Ball* b, std::vector<Ball*>& balls, int startIndex)
{
	for (int i = startIndex; i < balls.size(); i++)
	{
		checkCollision(*b, *balls[i]);
	}
}

void BallController::checkCollision(Ball& b1, Ball& b2)
{
	glm::vec2 distVec = b2.position - b1.position;
	glm::vec2 distDir = glm::normalize(distVec);
	float dist = glm::length(distVec);

	float totalRadius = b2.radius + b1.radius;

	float collisionDepth = totalRadius - dist;
	if (collisionDepth > 0)
	{
		if (b1.mass < b2.mass)
		{
			b1.position -= distDir * collisionDepth;
		}
		else
		{
			b2.position += distDir*collisionDepth;
		}

		float aci = glm::dot(b1.velocity, distDir);
		float bci = glm::dot(b2.velocity, distDir);

		float acf = (aci*(b1.mass - b2.mass) + 2 * b2.mass*bci) / (b1.mass + b2.mass);
		float bcf = (bci*(b2.mass - b1.mass) + 2 * b1.mass*aci) / (b1.mass + b2.mass);

		b1.velocity += (acf - aci) * distDir;
		b2.velocity += (bcf - bci) * distDir;
	}
}

bool BallController::isMouseOnBall(Ball& b, float mouseX, float mouseY)
{
	return (mouseX >= b.position.x - b.radius && mouseX < b.position.x + b.radius&&
			mouseY >= b.position.y - b.radius && mouseY < b.position.y + b.radius);
}

glm::vec2 BallController::getGravityAccel()
{
	const float GRAVITY_FORCE = 0.1f;
	glm::vec2 gravity;
	switch (_gravityDir)
	{
	case LEFT:
		gravity = glm::vec2(-GRAVITY_FORCE, 0.0f);
		break;
	case UP:
		gravity = glm::vec2(0.0f, GRAVITY_FORCE);
		break;
	case RIGHT:
		gravity = glm::vec2(GRAVITY_FORCE, 0.0f);
		break;
	case DOWN:
		gravity = glm::vec2(0.0f, -GRAVITY_FORCE);
		break;
	default:
		gravity = glm::vec2(0.0f);
		break;
	}

	return gravity;
}