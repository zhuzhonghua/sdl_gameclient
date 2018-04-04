#include "character.h"
#include "swftypes.h"
#include "movie.h"

#include <SDL.h>

Character::Character()
{
	_tex = NULL;
	_id = -1;
	_type = -1;
	_shape = NULL;
}

Character::~Character()
{
	if (_tex)
	{
		SDL_DestroyTexture(_tex);
	}

	if (_shape)
	{
		delete _shape;
	}	
}

void Character::update(Movie* m, swftypes::matrix mat, int ratio)
{
	switch (_type)
	{
	case SWFTAG::DEFINESHAPE2:
	case SWFTAG::DEFINESHAPE3:
	case SWFTAG::DEFINESHAPE:
		updateShape(m, mat, ratio);
		break;
	}
}

void Character::updateShape(Movie* m, swftypes::matrix mat, int ratio)
{
	const std::vector<swftypes::FillStyle>& fillStyles = _shape->fillStyles.fillStyles;
	for (int i = 0; i < fillStyles.size();++i)
	{
		const swftypes::FillStyle& fs = fillStyles[i];
		if (fs.fillStyleType == 0x40 || fs.fillStyleType == 0x41 ||
			fs.fillStyleType == 0x42 || fs.fillStyleType == 0x43)
		{
			SDL_Rect drect;
			_bounds.copy(&drect);

			Character* bm = m->getCharacter(fs.bitmapId);			
			if (bm)
			{
				bm->updateBitmap(m->getRender(), &drect);
			}
			else
			{
				// 65535
			}
		}
	}
}

void Character::updateBitmap(SDL_Renderer* r, SDL_Rect* drect)
{
	SDL_RenderCopy(r, _tex, NULL, drect);
}