#pragma once

#include "pseudopixel.h"
#include "util.h"

#include "emitter.h"

class PixelParticle :public PseudoPixel{
protected:
	float _sizef;

	float _lifespan;
	float _left;
public:
	PixelParticle();

	void reset(float x, float y, int c, float s, float lifespan);
	virtual void update();
};

class Shrinking :public PixelParticle {
public:
	Shrinking();
	virtual void update();
};

class SparkParticle :public PixelParticle{
public:
	static Emitter::Factory* FACTORY;

	SparkParticle();
	void reset(float x, float y);
	virtual void update();
};

class ElmoParticle :public Shrinking{
public:
	static Emitter::Factory* FACTORY;

	ElmoParticle();
	void reset(float x, float y);

	virtual void update();
};

class ShadowParticle :public Shrinking{
public:
	static Emitter::Factory* MISSILE;
	static Emitter::Factory* CURSE;
	static Emitter::Factory* UP;

	ShadowParticle();

	void reset(float x, float y);
	void resetCurse(float x, float y);
	void resetUp(float x, float y);
	virtual void update();
};