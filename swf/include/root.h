#ifndef __ROOT_H__
#define __ROOT_H__

#include "SDL.h"

#include <vector>

class Movie;
class Root
{
	friend class Movie;
public:
	Root();
	~Root();
	bool init(int height, int width);

	void update();
	void run();
	Movie* createMovie(const char* filename);
	void addPlayList(Movie* m);
private:
	std::vector<Movie*>	_allMovies;
	std::vector<Movie*>	_displayMovies;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
};
#endif