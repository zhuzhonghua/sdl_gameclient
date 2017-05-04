#include "bullet.h"
#include "resourcemanager.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float speed, int lifeTime)
{
	_position = position;
	_direction = direction;
	_speed = speed;
	_lifeTime = lifeTime;
}

Bullet::~Bullet()
{

}

void Bullet::draw(Zhu::SpriteBatch& batch)
{
	static Zhu::Texture tex = Zhu::ResourceManager::getTexture("data/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	Zhu::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 position = glm::vec4(_position.x, _position.y, 30,30);
	batch.draw(position, uv, tex.id, 0.0f, color);
}

bool Bullet::update()
{
	_position += _direction * _speed;
	_lifeTime--;
	if (_lifeTime <= 0)
		return true;
	return false;
}
