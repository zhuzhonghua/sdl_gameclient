#include "capsule.h"

void Capsule::init(b2World* world, const glm::vec2& pos, const glm::vec2 dimen, float density, float friction, bool fixRotation)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.fixedRotation = fixRotation;

	_body = world->CreateBody(&bodyDef);

	b2PolygonShape bodyBox;
	bodyBox.SetAsBox(dimen.x / 2.0f, (dimen.y-dimen.x) / 2.0f);
	//bodyBox.SetAsBox(dimen.x / 2.0f, dimen.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &bodyBox;
	fixtureDef.density = density;
	fixtureDef.friction = friction;

	_fixtures[0] = _body->CreateFixture(&fixtureDef);

	_dimen = dimen;

	b2CircleShape circleShape;
	circleShape.m_radius = dimen.x / 2.0f;

	b2FixtureDef circleDef;
	circleDef.shape = &circleShape;
	circleDef.density = density;
	circleDef.friction = friction;

	circleShape.m_p.Set(0.0f, (-_dimen.y + _dimen.x) / 2.0f);
	_fixtures[1] = _body->CreateFixture(&circleDef);

	circleShape.m_p.Set(0.0f, (_dimen.y - _dimen.x) / 2.0f);
	_fixtures[2] = _body->CreateFixture(&circleDef);
}

void Capsule::drawDebug(Zhu::DebugRenderer* debugRenderer)
{
	glm::vec4 destRect;
	destRect.x = _body->GetPosition().x - _dimen.x / 2.0f;
	destRect.y = _body->GetPosition().y - (_dimen.y-_dimen.x) / 2.0f;
	//destRect.y = _body->GetPosition().y - _dimen.y / 2.0f;
	destRect.z = _dimen.x;
	destRect.w = _dimen.y-_dimen.x;
	//destRect.w = _dimen.y;

	debugRenderer->drawBox(destRect, Zhu::Color(255, 255, 255, 255), _body->GetAngle());

	debugRenderer->drawCircle(glm::vec2(destRect.x + _dimen.x / 2.0f, destRect.y), Zhu::Color(255, 255, 255, 255), _dimen.x/2.0f);

	debugRenderer->drawCircle(glm::vec2(destRect.x + _dimen.x / 2.0f, destRect.y + destRect.w), Zhu::Color(255, 255, 255, 255), _dimen.x / 2.0f);
}