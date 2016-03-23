#ifndef __CHARACTER_H__
#define __CHARACTER_H__
#include "swftypes.h"
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class Movie;

class Character
{
public:
	Character();
	~Character();

	void	update(Movie* m, swftypes::matrix mat, int ratio);
	void	updateShape(Movie* m, swftypes::matrix mat, int ratio);
	void	updateBitmap(SDL_Renderer* r, SDL_Rect* drect);
public:
	int				_type;
	int				_id;

	// DefineJPEGBITS
	SDL_Texture*	_tex;

	// DefineShape
	swftypes::rect		_bounds;
	swftypes::ShapeWithStyle*	_shape;
};
#endif