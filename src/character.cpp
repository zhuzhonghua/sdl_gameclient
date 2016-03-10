#include "character.h"

#include <SDL.h>

Character::Character(SDL_Texture* tex, int type)
{
	_type = type;
	_tex = tex;
}

Character::~Character()
{
	SDL_DestroyTexture(_tex);
}