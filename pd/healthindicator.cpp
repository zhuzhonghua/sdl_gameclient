#include "healthindicator.h"
#include "image.h"
#include "texturecache.h"
#include "char.h"
#include "charsprite.h"

const float HealthIndicator::HEIGHT = 2;

HealthIndicator* HealthIndicator::instance;

HealthIndicator::HealthIndicator()
{
	target = NULL;

	init();

	instance = this;
}

void HealthIndicator::update()
{
	Component::update();

	if (target != NULL && target->isAlive() && target->sprite->visible) 
	{
		CharSprite* sprite = target->sprite;
		bg->scale.x = sprite->width;
		level->scale.x = sprite->width * target->HP / target->HT;
		bg->x = level->x = sprite->x;
		bg->y = level->y = sprite->y - HEIGHT - 1;

		visible = true;
	}
	else 
	{
		visible = false;
	}
}

void HealthIndicator::Target(Char* ch)
{
	if (ch != NULL && ch->isAlive()) 
	{
		target = ch;
	}
	else 
	{
		target = NULL;
	}
}

void HealthIndicator::createChildren()
{
	bg = new Image(TextureCache::createSolid(0xFFcc0000));
	bg->scale.y = HEIGHT;
	add(bg);

	level = new Image(TextureCache::createSolid(0xFF00cc00));
	level->scale.y = HEIGHT;
	add(level);
}
