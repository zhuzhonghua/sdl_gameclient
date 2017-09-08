#include "pixelparticle.h"
#include "util.h"
#include "game.h"

PixelParticle::PixelParticle()
{
	_sizef = 0;
	_lifespan = 0;
	_left = 0;
	GameMath::PointFSet(&origin, +0.5f);
	//origin.set(+0.5f);
}

void PixelParticle::reset(float x, float y, int c, float s, float lifespan)
{
	revive();

	this->x = x;
	this->y = y;

	color(c);
	size(this->_sizef = s);

	this->_left = this->_lifespan = lifespan;
}

void PixelParticle::update()
{
	PseudoPixel::update();

	if ((_left -= Game::elapsed) <= 0) {
		kill();
	}
}

Shrinking::Shrinking()
{
	tag = "Shrinking";
}

void Shrinking::update()
{
	PixelParticle::update();
	size(_sizef * _left / _lifespan);
}

namespace{
	class FactoryNew :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			SparkParticle* sp = (SparkParticle*)emitter->recycle("SparkParticle");
			if (sp == NULL)
			{
				sp = new SparkParticle();
				emitter->add(sp);
			}
			sp->reset(x, y);
		}

		virtual bool lightMode() { return true; };
	};
}

Emitter::Factory* SparkParticle::FACTORY = new FactoryNew();

SparkParticle::SparkParticle()
{
	tag = "SparkParticle";

	size(2);

	GameMath::PointFSet(&acc, 0, +50);
	//acc.set(0, +50);
}

void SparkParticle::reset(float x, float y)
{
	revive();

	this->x = x;
	this->y = y;

	_left = _lifespan = Random::Float(0.5f, 1.0f);

	GameMath::PointFPolar(&speed, -Random::Float(3.1415926f), Random::Float(20, 40));
	//speed.polar(-Random.Float(3.1415926f), Random.Float(20, 40));
}

void SparkParticle::update()
{
	PixelParticle::update();
	size(Random::Float(5 * _left / _lifespan));
}
