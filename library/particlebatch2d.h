#pragma once

#include "glm/glm.hpp"

#include "vertex.h"
#include "spritebatch.h"
#include "texture.h"

namespace Zhu{


class Particle2D{
public:
	Particle2D();
	friend class ParticleBatch2D;
	void update(float deltaTime);

	glm::vec2 position;
	glm::vec2 velocity;
	float width;
	Color color;
	float life;
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
	std::vector<Particle2D> _particles;
	int _lastFreeParticle;
	Texture _tex;
};


}