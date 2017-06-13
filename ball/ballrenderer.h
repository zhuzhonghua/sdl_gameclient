#pragma once

#include "spritebatch.h"
#include "glslprogram.h"
#include "ball.h"

#include <vector>

class BallRenderer{
public:
	virtual void renderBalls(Zhu::SpriteBatch& batch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix);
};