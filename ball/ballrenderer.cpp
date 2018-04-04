#include "ballrenderer.h"

void BallRenderer::renderBalls(Zhu::SpriteBatch& batch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	batch.begin();

	// Render all the balls
	for (int i = 0; i < balls.size();i++) 
	{
		const Ball& ball = balls[i];
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius, ball.radius * 2.0f, ball.radius * 2.0f);
		batch.draw(destRect, uvRect, ball.texId, 0.0f, ball.color);
	}

	batch.end();
	batch.renderBatch();
}
