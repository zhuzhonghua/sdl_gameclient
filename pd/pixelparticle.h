#pragma once

#include "pseudopixel.h"
#include "util.h"
#include "game.h"

class PixelParticle :public PseudoPixel{
protected:
	float _sizef;

	float _lifespan;
	float _left;
public:
	PixelParticle() 
	{
		_sizef = 0;
		_lifespan = 0;
		_left = 0;
		GameMath::PointFSet(&origin, +0.5f);
		//origin.set(+0.5f);
	}

	void reset(float x, float y, int c, float s, float lifespan) 
	{
		revive();

		this->x = x;
		this->y = y;

		color(c);
		size(this->_sizef = s);

		this->_left = this->_lifespan = lifespan;
	}
	virtual void update()
	{
		PseudoPixel::update();

		if ((_left -= Game::elapsed) <= 0) {
			kill();
		}
	}	
};

class Shrinking :public PixelParticle {
public:
	Shrinking()
	{
		tag = "Shrinking";
	}

	virtual void update() 
	{
		PixelParticle::update();
		size(_sizef * _left / _lifespan);
	}
};