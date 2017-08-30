#include "toolbar.h"
#include "image.h"
#include "dungeontilemap.h"
#include "item.h"
#include "game.h"
#include "util.h"

void Toolbar::Tool::createChildren()
{
	base = new Image(Assets::TOOLBAR);
	add(base);
}

void Toolbar::Tool::layout()
{
	Component::layout();

	base->x = _x;
	base->y = _y;
}

void Toolbar::Tool::onTouchDown()
{
	base->brightness(1.4f);
}

void Toolbar::Tool::onTouchUp()
{
	if (active) 
	{
		base->resetColor();
	}
	else 
	{
		base->tint(BGCOLOR, 0.7f);
	}
}

Toolbar::Tool::Tool(int x, int y, int width, int height)
{
	init();

	base->frame(x, y, width, height);

	this->_width = width;
	this->_height = height;
}

void Toolbar::Tool::enable(bool value)
{
	if (value != active) 
	{
		if (value) 
		{
			base->resetColor();
		}
		else 
		{
			base->tint(BGCOLOR, 0.7f);
		}
		active = value;
	}
}

const float Toolbar::PickedUpItem::DISTANCE = DungeonTilemap::SIZE;
const float Toolbar::PickedUpItem::DURATION = 0.2f;

Toolbar::PickedUpItem::PickedUpItem()
{
	originToCenter();

	active = visible = false;
}

void Toolbar::PickedUpItem::reset(Item* item, float dstX, float dstY)
{
	view(item->Image(), item->glowing());

	active = visible = true;

	this->dstX = dstX - ItemSprite::SIZE / 2;
	this->dstY = dstY - ItemSprite::SIZE / 2;
	left = DURATION;

	x = this->dstX - DISTANCE;
	y = this->dstY - DISTANCE;
	alpha(1);
}

void Toolbar::PickedUpItem::update()
{
	ItemSprite::update();

	if ((left -= Game::elapsed) <= 0) 
	{

		visible = active = false;

	}
	else 
	{
		float p = left / DURATION;
		GameMath::PointFSet(&scale, (float)std::sqrt(p));
		float offset = DISTANCE * p;
		x = dstX - offset;
		y = dstY - offset;
	}
}
