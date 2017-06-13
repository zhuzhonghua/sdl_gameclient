#pragma once

#include "window.h"
#include "camera2d.h"
#include "glslprogram.h"
#include "spritebatch.h"
#include "inputmanager.h"
#include "timing.h"
#include "spritefont.h"
#include "ballrenderer.h"
#include "ballcontroller.h"
#include "grid.h"

enum GameState
{
	PLAY,
	EXIT
};

class Game{
public:
	Game();

	void init();
	void run();
	void exit();

	void initRenderers();
	void initBalls();
	void drawGame();
	void drawHUD();
	void processInput();
	void update(float deltaTime);

	const int WIN_WIDTH = 1024;
	const int WIN_HEIGHT = 768;

	GameState	state;

private:
	Zhu::Window* _window;
	Zhu::Camera2D* _camera;
	Zhu::Camera2D* _hudCamera;

	Zhu::GLSLProgram* _colorProgram;
	Zhu::SpriteFont* _spriteFont;
	Zhu::SpriteBatch _spriteBatch;
	Zhu::SpriteBatch _hudSpriteBatch;

	Zhu::InputManager _inputMgr;
	Zhu::FPSLimiter _fpsLimiter;

	BallController _ballCtl;

	Grid* _grid;
	std::vector<Ball> _balls;
	std::vector<BallRenderer*> _ballRenderers;
	int _currentRenderer;

	float _fps;

	float _time;
	float _frameTime;
};