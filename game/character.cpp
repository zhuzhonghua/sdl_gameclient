#include "stdafx.h"

#include "character.h"
#include "global.h"
#include "camera.h"

Character::Character()
{
	rend = Global::getRenderer();
	tex = IMG_LoadTexture(rend, "character.png");
	rect.x = 0;
	rect.y = 0;
	rect.w = 94;
	rect.h = 94;

	ani = false;
}

Character::~Character()
{
	SDL_DestroyTexture(tex);
}

void Character::aniToPos(int x, int y)
{
	ani = true;
	dest.x = x;
	dest.y = y;
}

void Character::setPosition(int px, int py)
{
	GameObject::setPosition(px, py);
	ani = false;
}

void Character::update()
{
	if (ani)
	{
		int dx = dest.x - rect.x;
		int dy = dest.y - rect.y;

		if (dx*dx+dy*dy > 1)
		{
			int xDir = dx > 0 ? 1 : -1;
			if (dx == 0)
			{
				xDir = 0;
			}
			int yDir = dy > 0 ? 1 : -1;
			if (dy == 0)
			{
				yDir = 0;
			}

			rect.x += xDir;
			rect.y += yDir;
		}
		else
		{
			ani = false;
		}
	}
}

void Character::draw()
{
	SDL_Rect tmp = rect;
	tmp.x = rect.x - MAINCAMERA()->getX();
	tmp.y = rect.y - MAINCAMERA()->getY();

	SDL_RenderCopy(rend, tex, NULL, &tmp);
}

void Character::handleEvent(SDL_Event* e)
{
	if (e->type == SDL_MOUSEBUTTONDOWN &&
		e->button.button == SDL_BUTTON_LEFT)
	{
		int x = e->button.x;
		int y = e->button.y;

		SDL_Point p = { x, y };
		p = MAINCAMERA()->screenToWorld(&p);

		aniToPos(p.x, p.y);
	}
}