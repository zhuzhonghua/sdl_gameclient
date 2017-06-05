#pragma once

#include "glm/glm.hpp"

#include "vertex.h"
#include "spritebatch.h"
#include "texture.h"

namespace Zhu{


class Particle{
public:
	Particle();
	friend class ParticleBatch2D;
	void update(float deltaTime);
private:
	glm::vec2 _position;
	glm::vec2 _velocity;
	float _width;
	Color _color;
	float _life;
};

class ParticleBatch2D{
public:
	ParticleBatch2D();
	~ParticleBatch2D();

	void init(int maxNum, float decayRate, Texture tex);

	void update(float deltaTime);
	void draw(SpriteBatch* batch);

	void addParticle(const glm::vec2& position,
					const glm::vec2& velocity,
					const Color& color,
					float width);

private:
	int findFreeParticle();

	float _decayRate;
	std::vector<Particle> _particles;
	int _lastFreeParticle;
	Texture _tex;
};


}