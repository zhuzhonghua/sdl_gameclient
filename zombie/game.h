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
#include "level.h"
#include "player.h"
#include "zombie.h"
#include "bullet.h"
#include "spritefont.h"

enum GameState
{
	PLAY,
	EXIT
};


const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 5.0f;

class Game
{
	static Game* _inst;
	Game();
	
	Zhu::SpriteBatch _spriteBatch;
	Zhu::SpriteBatch _hudSpriteBatch;

	Zhu::Window* _window;

	Zhu::Camera2D* _camera;
	Zhu::Camera2D* _hudCamera;

	Zhu::InputManager _inputMgr;
	Zhu::FPSLimiter _fpsLimiter;

	Zhu::SpriteFont* _spriteFont;

	Zhu::GLSLProgram* _colorProgram;

	float _fps;

	float _time;
	float _frameTime;

	std::vector<Level*> _levels;
	int _currentLevel;

	Player* _player;
	std::vector<Human*> _humans;
	std::vector<Zombie*> _zombies;
	std::vector<Bullet> _bullets;

public:
	
	const int WIN_WIDTH = 1024;
	const int WIN_HEIGHT = 768;

	GameState	state;
public:	
	~Game();
	static Game* inst();
	void init();
	void run();
	void exit();

	void initShaders();
	void initLevels();

	void checkVictory();
	void updateBullets(float deltaTime);
	void updateAgents(float deltaTime);
	void processInput();
	void drawGame();
	void drawHud();
};
#endif