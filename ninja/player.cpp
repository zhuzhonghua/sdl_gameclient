#include "player.h"
#include "resourcemanager.h"

Player::Player()
{
	_onGround = false;
	_direction = 1;
	_animateTime = 0;
	_runState = PlayerRunState::STANDDING;
	_isPunching = false;
}

Player::~Player()
{

}

void Player::init(b2World* world, const glm::vec2& pos, const glm::vec2& drawDims, const glm::vec2& collisionDims, Zhu::Color color, bool fixRotation)
{
	Zhu::Texture tex = Zhu::ResourceManager::getTexture("data/ninja/blue_ninja.png");

	//_box.init(world, pos, dimen, tex, color, fixRotation, glm::vec4(0.0f,0.0f,0.1f,0.5f));
	_capsule.init(world, pos, collisionDims, 1.0f, 0.1f, true);
	_color = color;
	_drawDims = drawDims;

	_tex.init(tex, glm::ivec2(10, 2));
}

void Player::draw(Zhu::SpriteBatch* sb)
{
	//_box.draw(sb);
	glm::vec4 destRect;
	destRect.x = _capsule.getBody()->GetPosition().x - _drawDims.x / 2.0f;
	destRect.y = _capsule.getBody()->GetPosition().y - _capsule.getDimen().y / 2.0f;
	destRect.z = _drawDims.x;
	destRect.w = _drawDims.y;

	float animateSpeed = 0.2f;
	int tileIdx;
	int numTiles;
	glm::vec2 velocity;
	velocity.x = _capsule.getBody()->GetLinearVelocity().x;
	velocity.y = _capsule.getBody()->GetLinearVelocity().y;

	if (_onGround)
	{
		if (_isPunching)
		{
			numTiles = 4;
			tileIdx = 1;
			_runState = PlayerRunState::PUNCHING;
		}
		else if (std::abs(velocity.x) >= 1.0f && ((velocity.x > 0 && _direction > 0) || (velocity.x < 0 && _direction < 0)))
		{
			numTiles = 6;
			tileIdx = 10;

			animateSpeed = std::abs(velocity.x)*0.025f;

			if (_runState != PlayerRunState::RUNNING)
			{
				_runState = PlayerRunState::RUNNING;
				_animateTime = 0;
			}
		}
		else
		{
			tileIdx = 0;
			numTiles = 1;

			_runState = PlayerRunState::STANDDING;
		}		
	}
	else
	{
		if (_isPunching)
		{
			numTiles = 1;
			tileIdx = 18;
			_runState = PlayerRunState::PUNCHING;
			animateSpeed *= 0.25f;
		}
		else if (std::abs(velocity.x) > 10.0f)
		{
			numTiles = 1;
			tileIdx = 10;

			_runState = PlayerRunState::INAIR;
		}
		else if (velocity.y <= 0.0f)
		{
			numTiles = 1;
			tileIdx = 17;

			_runState = PlayerRunState::INAIR;
		}
		else
		{
			numTiles = 1;
			tileIdx = 16;

			_runState = PlayerRunState::INAIR;
		}
	}

	if (_animateTime > numTiles)
	{
		_isPunching = false;
	}
	_animateTime += animateSpeed;

	glm::vec4 uvRect = _tex.getUVs(tileIdx + (int)_animateTime % numTiles);
	if (_direction == -1)
	{
		uvRect.x += 1.0f / _tex.dims.x;
		uvRect.z *= -1;
	}
	
	sb->draw(destRect, uvRect, _tex.tex.id, 0.0f, _color, _capsule.getBody()->GetAngle());
}

void Player::drawDebug(Zhu::DebugRenderer* debugRenderer)
{
	_capsule.drawDebug(debugRenderer);
}

void Player::update(Zhu::InputManager* inputMgr)
{
	b2Body* body = _capsule.getBody();

	if (inputMgr->isKeyDown(SDLK_a))
	{
		_direction = -1;
		body->ApplyForceToCenter(b2Vec2(-50.0f, 0.0f), true);
	}
	else if (inputMgr->isKeyDown(SDLK_d))
	{
		_direction = 1;
		body->ApplyForceToCenter(b2Vec2(50.0f, 0.0f), true);
	}
	else
	{
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x*0.9f, body->GetLinearVelocity().y));
	}

	if (inputMgr->isKeyDown(SDLK_SPACE))
	{
		_isPunching = true;
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

	_onGround = false;
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
				if (manifold.points[i].y < body->GetPosition().y - _capsule.getDimen().y / 2.0f + _capsule.getDimen().x / 2.0f + 0.01f)
				{
					below = true;
					break;
				}
			}

			if (below)
			{
				_onGround = true;
				if (inputMgr->isKeyPressed(SDLK_w))
				{
					body->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), b2Vec2(0.0f, 0.0f), true);
					break;
				}
			}
		}
	}
}