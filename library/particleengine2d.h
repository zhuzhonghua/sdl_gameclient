#pragma once

#include <vector>

#include "particlebatch2d.h"
#include "spritebatch.h"

namespace Zhu{
class ParticleBatch2D;

class ParticleEngine2D{
public:
	ParticleEngine2D();
	~ParticleEngine2D();

	void addParticleBatch(ParticleBatch2D* batch);
	void update(float deltaTime);
	void draw(SpriteBatch* batch);
private:
	std::vector<ParticleBatch2D*> _batches;
};
}