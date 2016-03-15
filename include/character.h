#ifndef __CHARACTER_H__
#define __CHARACTER_H__
#include "swftypes.h"
struct SDL_Texture;

class Character
{
public:
	Character();
	~Character();

public:
	int				_type;
	int				_id;

	// DefineJPEGBITS
	SDL_Texture*	_tex;

	// DefineShape
	swftypes::rect		_bounds;
	swftypes::Shape*	_shape;
};
#endif