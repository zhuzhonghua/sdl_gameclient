#pragma once

namespace Zengine{
class Scene;
class App{
public:
	static App* inst;

	App();	
	virtual ~App();

	virtual void init(const std::string& tile, int w = 640, int h = 960);
	virtual void run();
	void processInput();
	void update();
	void draw();

public:
	static int width;
	static int height;

	static float timeScale;
	static float elapsed;


	enum State
	{
		PLAY,
		EXIT
	};
	
	State state;

protected:
	SDL_Window* _window;
	SDL_GLContext _context;

	std::vector<SDL_Event>	_motionEvents;
	float	_fps;

	long	_now;
	long	_step;

	Scene*	_scene;
};
};