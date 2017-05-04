#include "agent.h"

#include "resourcemanager.h"

Agent::Agent()
{

}
Agent::~Agent()
{

}

void Agent::draw(Zhu::SpriteBatch& spriteBatch)
{
	static int texID = Zhu::ResourceManager::getTexture("data/zombie/Textures/circle.png").id;

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;

	spriteBatch.draw(destRect, uvRect, texID, 0, _color);
}