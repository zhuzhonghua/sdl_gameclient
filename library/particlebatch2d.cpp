#include "particlebatch2d.h"

using namespace Zhu;

Particle::Particle()
{
	_life = 0.0f;
	_width = 0.0f;
	_position = glm::vec2(0.0);
	_velocity = glm::vec2(0.0);
}

void Particle::update(float deltaTime)
{
	_position += _velocity*deltaTime;
}

ParticleBatch2D::ParticleBatch2D()
{
	_decayRate = 0.1f;
}

ParticleBatch2D::~ParticleBatch2D()
{

}

void ParticleBatch2D::init(int maxNum, float decayRate,Texture tex)
{
	_particles.resize(maxNum);
	_decayRate = decayRate;
	_tex = tex;
	_lastFreeParticle = 0;
}

void ParticleBatch2D::addParticle(const glm::vec2& position, const glm::vec2& velocity, const Color& color,float width)
{
	int idx = findFreeParticle();

	_particles[idx]._color = color;
	_particles[idx]._position = position;
	_particles[idx]._width = width;
	_particles[idx]._velocity = velocity;
	_particles[idx]._life = 1.0f;
}

int ParticleBatch2D::findFreeParticle()
{
	for (int i = _lastFreeParticle; i < _particles.size(); i++)
	{
		if (_particles[i]._life <= 0.0f)
		{
			_lastFreeParticle = i;
			return _lastFreeParticle;
		}
	}
	for (int i = 0; i < _lastFreeParticle; i++)
	{
		if (_particles[i]._life <= 0.0f)
		{
			_lastFreeParticle = i;
			return _lastFreeParticle;
		}
	}
	return 0;
}

void ParticleBatch2D::update(float deltaTime)
{
	for (int i = 0; i < _particles.size(); i++)
	{
		if (_particles[i]._life > 0.0f)
		{
			_particles[i].update(deltaTime);
			_particles[i]._life -= _decayRate*deltaTime;
		}
	}
}

void ParticleBatch2D::draw(SpriteBatch* batch)
{
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	for (int i = 0; i < _particles.size(); i++)
	{
		if (_particles[i]._life > 0.0f)
		{
			glm::vec4 destRect(_particles[i]._position.x, _particles[i]._position.y, _particles[i]._width, _particles[i]._width);
			batch->draw(destRect, uvRect, _tex.id, 0.0f, _particles[i]._color);
		}
	}
}