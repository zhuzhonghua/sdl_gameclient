#pragma once

#include "window.h"
#include "camera2d.h"
#include "glslprogram.h"
#include "spritebatch.h"
#include "sprite.h"
#include "inputmanager.h"
#include "timing.h"
#include "spritefont.h"
#include "Box2D/Box2D.h"
#include "debugrenderer.h"
#include "texture.h"
#include "box.h"
#include "player.h"

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
	Zhu::GLSLProgram* _lightProgram;
	Zhu::SpriteFont* _spriteFont;
	Zhu::SpriteBatch _spriteBatch;
	Zhu::SpriteBatch _hudSpriteBatch;

	Zhu::Sprite	_sp;

	Zhu::InputManager _inputMgr;
	Zhu::FPSLimiter _fpsLimiter;

	float _fps;

	float _time;
	float _frameTime;

	b2World*	_world;
	std::vector<Box> _boxes;
	Player	_player;

	Zhu::Texture _tex;

	Zhu::DebugRenderer _debugRenderer;
};