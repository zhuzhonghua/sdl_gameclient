#include "particlebatch2d.h"

using namespace Zhu;

Particle2D::Particle2D()
{
	life = 0.0f;
	width = 0.0f;
	position = glm::vec2(0.0);
	velocity = glm::vec2(0.0);
}

void Particle2D::update(float deltaTime)
{
	position += velocity*deltaTime;
	color.a = life*255.0f;
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

	_particles[idx].color = color;
	_particles[idx].position = position;
	_particles[idx].width = width;
	_particles[idx].velocity = velocity;
	_particles[idx].life = 1.0f;
}

int ParticleBatch2D::findFreeParticle()
{
	for (int i = _lastFreeParticle; i < _particles.size(); i++)
	{
		if (_particles[i].life <= 0.0f)
		{
			_lastFreeParticle = i;
			return _lastFreeParticle;
		}
	}
	for (int i = 0; i < _lastFreeParticle; i++)
	{
		if (_particles[i].life <= 0.0f)
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
		if (_particles[i].life > 0.0f)
		{
			_particles[i].update(deltaTime);
			_particles[i].life -= _decayRate*deltaTime;
		}
	}
}

void ParticleBatch2D::draw(SpriteBatch* batch)
{
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	for (int i = 0; i < _particles.size(); i++)
	{
		if (_particles[i].life > 0.0f)
		{
			glm::vec4 destRect(_particles[i].position.x, _particles[i].position.y, _particles[i].width, _particles[i].width);
			batch->draw(destRect, uvRect, _tex.id, 0.0f, _particles[i].color);
		}
	}
}