#ifndef __SWFDISPLAY_H__
#define __SWFDISPLAY_H__

#include "swftypes.h"
class Movie;
class Character;

class SWFDisplay
{
public:
	SWFDisplay();
public:
	bool operator < (const SWFDisplay& ti) const 
	{
		return depth < ti.depth;
	}

	bool operator == (const SWFDisplay& ti) const
	{
		return depth == ti.depth;
	}
	void		update(Movie* m);
public:
	int			depth;
	Character*	ch;

	swftypes::cxform	cx;
	swftypes::matrix	mat;
	int					ratio;
	int					clipDepth;
	std::string			name;
};
#endif