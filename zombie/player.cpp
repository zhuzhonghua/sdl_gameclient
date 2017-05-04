#include "player.h"

#include "SDL.h"

Player::Player()
{

}
Player::~Player()
{

}

void Player::init(float speed, glm::vec2 pos, Zhu::InputManager* inputMgr)
{
	_inputMgr = inputMgr;

	_speed = speed;
	_position = pos;

	_color.r = 0;
	_color.g = 0;
	_color.b = 185;
	_color.a = 255;
}

void Player::update()
{
	if (_inputMgr->isKeyPressed(SDLK_w))
	{
		_position.y += _speed;
	}
	else if (_inputMgr->isKeyPressed(SDLK_s))
	{
		_position.y -= _speed;
	}
	else if (_inputMgr->isKeyPressed(SDLK_a))
	{
		_position.x -= _speed;
	}
	else if (_inputMgr->isKeyPressed(SDLK_s))
	{
		_position.x += _speed;
	}
}