#ifndef __MOVIE_H__
#define __MOVIE_H__
#include "swftypes.h"

#include <map>

class Root;
class Loader;
class Character;
struct SDL_Renderer;

using namespace swftypes;

class Movie
{
public:
	Movie(Root* r);
	~Movie();
	bool load(const char* name);
	void play();
	void advance();
	void update();

	int		get_frame_count() const { return _frame_count; }
	void	set_frame_count(int frames) { _frame_count = Max(frames, 1); }

	void	addCharacter(int id, Character* ch);
	Loader* getLoader() { return _l; }
	SDL_Renderer*	getRender();
private:
	void readHeader();
	void readTags();
private:
	int		_version;
	rect	_frame_size;
	float	_frame_rate;
	int		_frame_count;
private:
	swftypes::rgba	_bg;
private:
	Root*	_r;
	Loader* _l;

	std::map<int, Character*>	_chs;
};

#endif