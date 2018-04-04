#ifndef __GAME_H__
#define __GAME_H__

#include "sprite.h"
#include "glslprogram.h"
#include <vector>
#include "window.h"
#include "camera2d.h"
#include "spritebatch.h"
#include "inputmanager.h"
#include "timing.h"
#include "bullet.h"

enum GameState
{
	PLAY,
	EXIT
};

class Game
{
	static Game* _inst;
	Game();
	
	Zhu::SpriteBatch _spriteBatch;
	Zhu::Window* _window;
	Zhu::Camera2D* _camera;
	Zhu::InputManager _inputMgr;
	Zhu::FPSLimiter _fpsLimiter;

	std::vector<Bullet> _bullets;

	Zhu::GLSLProgram* _colorProgram;

	float _fps;

	float _time;
	float _frameTime;
public:
	
	const int WIN_WIDTH = 960;
	const int WIN_HEIGHT = 640;

	GameState	state;
public:	
	~Game();
	static Game* inst();
	void init();
	void run();
	void exit();

	void initShaders();

	void processInput();
	void drawGame();
};
#endif