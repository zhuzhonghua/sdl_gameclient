#include "character.h"
#include "swftypes.h"

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