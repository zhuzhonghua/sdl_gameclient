#include "player.h"

#include "resourcemanager.h"
#include "gun.h"

#include "SDL.h"

Player::Player()
{
	_currGunIdx = -1;
}
Player::~Player()
{

}

void Player::init(float speed, glm::vec2 pos, Zhu::InputManager* inputMgr, Zhu::Camera2D* camera, std::vector<Bullet>* bullets)
{
	_inputMgr = inputMgr;
	_camera = camera;
	_bullets = bullets;

	_speed = speed;
	_position = pos;

	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;

	_health = 150;

	_texID = Zhu::ResourceManager::getTexture("data/zombie/Textures/player.png").id;
}

void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zobies,
	float deltaTime)
{
	if (_inputMgr->isKeyDown(SDLK_w))
	{
		_position.y += _speed * deltaTime;
	}
	else if (_inputMgr->isKeyDown(SDLK_s))
	{
		_position.y -= _speed * deltaTime;
	}
	else if (_inputMgr->isKeyDown(SDLK_a))
	{
		_position.x -= _speed * deltaTime;
	}
	else if (_inputMgr->isKeyDown(SDLK_d))
	{
		_position.x += _speed * deltaTime;
	}

	if (_inputMgr->isKeyPressed(SDLK_1) && _guns.size() > 0)
	{
		_currGunIdx = 0;
	}
	else if (_inputMgr->isKeyPressed(SDLK_2) && _guns.size() > 1)
	{
		_currGunIdx = 1;
	}
	else if (_inputMgr->isKeyPressed(SDLK_3) && _guns.size() > 2)
	{
		_currGunIdx = 2;
	}

	glm::vec2 mouseCoords = _inputMgr->getMouseCoords();
	mouseCoords = _camera->convertScreenToWorld(mouseCoords);

	glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);

	_direction = glm::normalize(mouseCoords - centerPosition);

	if (_currGunIdx != -1)
	{
		_guns[_currGunIdx]->update(_inputMgr->isKeyDown(SDL_BUTTON_LEFT), centerPosition, _direction, *_bullets, deltaTime);
	}

	collideWithLevel(levelData);
}

void Player::addGun(Gun* gun)
{
	_guns.push_back(gun);

	if (_currGunIdx == -1)
		_currGunIdx = 0;
}