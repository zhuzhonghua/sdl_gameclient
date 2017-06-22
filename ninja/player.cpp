#include "player.h"
#include "resourcemanager.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::init(b2World* world, const glm::vec2& pos, const glm::vec2 dimen, Zhu::Color color, bool fixRotation)
{
	Zhu::Texture tex = Zhu::ResourceManager::getTexture("data/ninja/blue_ninja.png");

	_box.init(world, pos, dimen, tex, color, fixRotation, glm::vec4(0.0f,0.0f,0.1f,0.5f));
}

void Player::draw(Zhu::SpriteBatch* sb)
{
	_box.draw(sb);
}

void Player::update(Zhu::InputManager* inputMgr)
{
	b2Body* body = _box.getBody();

	if (inputMgr->isKeyDown(SDLK_a))
	{
		body->ApplyForceToCenter(b2Vec2(-50.0f, 0.0f), true);
	}
	else if (inputMgr->isKeyDown(SDLK_d))
	{
		body->ApplyForceToCenter(b2Vec2(50.0f, 0.0f), true);
	}
	else
	{
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x*0.9f, body->GetLinearVelocity().y));
	}

	const float MAX_SPEED = 10.0f;
	if (body->GetLinearVelocity().x < -MAX_SPEED)
	{
		body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > MAX_SPEED)
	{
		body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
	}

	for (b2ContactEdge* ce = body->GetContactList(); ce != NULL; ce = ce->next)
	{
		b2Contact* ca = ce->contact;
		if (ca->IsTouching())
		{
			b2WorldManifold manifold;
			ca->GetWorldManifold(&manifold);

			bool below = false;
			for (int i = 0; i < b2_maxManifoldPoints; i++)
			{
				if (manifold.points[i].y < body->GetPosition().y - _box.getDimen().y / 2.0f + 0.01f)
				{
					below = true;
					break;
				}
			}

			if (below)
			{
				if (inputMgr->isKeyPressed(SDLK_w))
				{
					body->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), b2Vec2(0.0f, 0.0f), true);
					break;
				}
			}
		}
	}
}