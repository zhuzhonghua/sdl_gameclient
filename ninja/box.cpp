#include "box.h"

Box::Box()
{
	_body = NULL;
	_fixture = NULL;
}

Box::~Box()
{

}

void Box::init(b2World* world, const glm::vec2& pos, const glm::vec2 dimen, Zhu::Texture tex, Zhu::Color color, bool fixRotation, glm::vec4 uvRect)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.fixedRotation = fixRotation;

	_body = world->CreateBody(&bodyDef);

	b2PolygonShape bodyBox;
	bodyBox.SetAsBox(dimen.x/2.0f, dimen.y/2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &bodyBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	
	_fixture = _body->CreateFixture(&fixtureDef);

	_dimen = dimen;
	_color = color;
	_tex = tex;
	_uvRect = uvRect;
}

void Box::draw(Zhu::SpriteBatch* sb)
{
	glm::vec4 destRect;
	destRect.x = _body->GetPosition().x - _dimen.x / 2.0f;
	destRect.y = _body->GetPosition().y - _dimen.y / 2.0f;
	destRect.z = _dimen.x;
	destRect.w = _dimen.y;

	sb->draw(destRect, _uvRect, _tex.id, 0.0f, _color, _body->GetAngle());
}