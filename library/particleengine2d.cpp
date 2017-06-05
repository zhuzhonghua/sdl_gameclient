#include "particleengine2d.h"

using namespace Zhu;

ParticleEngine2D::ParticleEngine2D()
{

}

ParticleEngine2D::~ParticleEngine2D()
{
	for (int i = 0; i < _batches.size(); i++)
	{
		delete _batches[i];
	}
	_batches.clear();
}

void ParticleEngine2D::addParticleBatch(ParticleBatch2D* batch)
{
	_batches.push_back(batch);
}

void ParticleEngine2D::update(float deltaTime)
{
	for (int i = 0; i < _batches.size(); i++)
	{
		_batches[i]->update(deltaTime);
	}
}

void ParticleEngine2D::draw(SpriteBatch* batch)
{
	for (int i = 0; i < _batches.size();i++)
	{
		batch->begin();
		
		_batches[i]->draw(batch);

		batch->end();
		batch->renderBatch();
	}	
}