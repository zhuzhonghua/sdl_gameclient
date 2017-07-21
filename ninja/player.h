#pragma once

#include "texture.h"
#include "spritebatch.h"
#include "Box2D/Box2D.h"

#include "box.h"
#include "glm/glm.hpp"
#include "inputmanager.h"
#include "debugrenderer.h"
#include "capsule.h"
#include "tilesheet.h"

enum PlayerRunState{
	STANDDING,
	RUNNING,
	PUNCHING,
	INAIR
};
class Player
{
public:
	Player();
	~Player();

	void init(b2World* world, const glm::vec2& pos, const glm::vec2& drawDims, const glm::vec2& collisionDims, Zhu::Color color, bool fixRotation);
	void draw(Zhu::SpriteBatch* sb);
	void drawDebug(Zhu::DebugRenderer* debugRenderer);

	void update(Zhu::InputManager* inputMgr);

	//Box& getBox() { return _box; }
	const Capsule& getCapsule() const{ return _capsule; }
	glm::vec2 getPosition() const{
		glm::vec2 rv;
		rv.x = _capsule.getBody()->GetPosition().x;
		rv.y = _capsule.getBody()->GetPosition().y;
		return rv;
	}
private:
	//Box _box;
	Capsule _capsule;
	Zhu::TileSheet _tex;
	Zhu::Color _color;
	glm::vec2 _drawDims;

	PlayerRunState _runState;
	float _animateTime;
	int _direction;
	bool _onGround;
	bool _isPunching;
};