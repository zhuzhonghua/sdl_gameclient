#include "stdafx.h"

#include "game.h"
#include "input.h"
#include "imageloader.h"

#include "engine.h"

#include "resourcemanager.h"

#include "gun.h"

#include <random>
#include <ctime>
#include <algorithm>

Game* Game::_inst;

Game::Game()
{
	state = GameState::PLAY;
	_time = 0;

	_camera = new Zhu::Camera2D();
	_camera->init(WIN_WIDTH, WIN_HEIGHT);

	_hudCamera = new Zhu::Camera2D();
	_hudCamera->init(WIN_WIDTH, WIN_HEIGHT);
	_hudCamera->setPosition(glm::vec2(WIN_WIDTH / 2, WIN_HEIGHT/2));
	_player = NULL;
}

Game::~Game()
{

}

Game* Game::inst()
{
	if (_inst == NULL)
	{
		_inst = new Game();
	}
	return _inst;
}

void Game::initShaders()
{
	_colorProgram = new Zhu::GLSLProgram();

	_colorProgram->compileShaders("data/Shaders/colorShading.vert", "data/Shaders/colorShading.frag");
	_colorProgram->addAttribute("vertexPosition");
	_colorProgram->addAttribute("vertexColor");
	_colorProgram->addAttribute("vertexUV");
	_colorProgram->linkShaders();
}
void Game::init()
{
	Zhu::init();

	_audioEngine.init();

	_window = new Zhu::Window();
	_window->create("Game1", WIN_WIDTH, WIN_HEIGHT);

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	initShaders();

	_spriteBatch.init();
	_hudSpriteBatch.init();

	_spriteFont = new Zhu::SpriteFont("data/Fonts/chintzy.ttf", 64);

	_fpsLimiter.init(60);

	initLevels();
}

void Game::initLevels()
{
	_levels.push_back(new Level("data/Zombie/Levels/level1.txt"));
	_currentLevel = 0;

	_player = new Player();
	_player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPos(), &_inputMgr, _camera, &_bullets);

	_humans.push_back(_player);

	std::mt19937 randomEngine;
	randomEngine.seed(time(NULL));
	std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth()-2);
	std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight() - 2);


	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++)
	{
		_humans.push_back(new Human());
		glm::vec2 pos(randX(randomEngine)*TILE_WIDTH, randY(randomEngine)*TILE_WIDTH);
	
		_humans.back()->init(HUMAN_SPEED, pos);
	}

	const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getZombieStartPos();
	for (int i = 0; i < zombiePositions.size(); i++)
	{
		_zombies.push_back(new Zombie());
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	const float BULLET_SPEED = 10.0f;

	_player->addGun(new Gun("Magnum", 10, 1, 5.0f, 30, BULLET_SPEED, _audioEngine.loadSoundEffect("data/Sound/shots/pistol.wav")));
	_player->addGun(new Gun("Shotgun", 30, 12, 20.0f, 4, BULLET_SPEED, _audioEngine.loadSoundEffect("data/Sound/shots/shotgun.wav")));
	_player->addGun(new Gun("MP5", 2, 1, 10.0f, 20, BULLET_SPEED, _audioEngine.loadSoundEffect("data/Sound/shots/cg1.wav")));
}

void Game::run()
{
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / 60;
	const int MAX_PHYSICS_STEPS = 6;
	const float MAX_DELTA_TIME = 1.0f;

	int previousTick = SDL_GetTicks();

	_camera->setScale(1.0f/3.0f);

	Zhu::Music music = _audioEngine.loadMusic("data/Sound/XYZ.ogg");
	music.play();

	SDL_StartTextInput();	
	while (state == GameState::PLAY)
	{
		_fpsLimiter.begin();

		float frameTime = SDL_GetTicks() - previousTick;
		previousTick = SDL_GetTicks();

		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		_time += 0.01;

		_inputMgr.update();
		processInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i++< MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			totalDeltaTime -= deltaTime;

			updateAgents(deltaTime);
			updateBullets(deltaTime);
		}
		

		_camera->setPosition(_player->getPosition());
		_camera->update();
		_hudCamera->update();

		drawGame();
		
		_fps = _fpsLimiter.end();
	
	}
	SDL_StopTextInput();
}

void Game::updateAgents(float deltaTime)
{
	for (int i = 0; i < _humans.size();i++)
	{
		_humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
	}

	for (int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
	}
	
	for (int i = 0; i < _zombies.size(); i++)
	{
		for (int j = i + 1; j < _zombies.size(); j++)
		{
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
	
		for (int j = 1; j < _humans.size(); j++)
		{
			if (_zombies[i]->collideWithAgent(_humans[j]))
			{
				_zombies.push_back(new Zombie());
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
	
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}
	
		if (_zombies[i]->collideWithAgent(_player))
		{
			fatalError("You Lose");
		}
	}
	
	for (int i = 0; i < _humans.size(); i++)
	{
		for (int j = i + 1; j < _humans.size(); j++)
		{
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}
}

void Game::updateBullets(float deltaTime)
{
	for (int i = 0; i < _bullets.size();)
	{
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime))
		{
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else
		{
			i++;
		}
	}

	bool wasBulletRemove = false;
	for (int i = 0; i < _bullets.size();i++)
	{
		wasBulletRemove = false;
		for (int j = 0; j < _zombies.size();)
		{
			if (_bullets[i].collideWithAgent(_zombies[j]))
			{
				if (_zombies[j]->applyDamage(_bullets[i].getDamage()))
				{
					delete _zombies[j];

					_zombies[j] = _zombies.back();
					_zombies.pop_back();
				}
				else
				{
					j++;
				}
				_bullets[i] = _bullets.back();
				_bullets.pop_back();

				wasBulletRemove = true;
				i--;
				break;
			}
			else
			{
				j++;
			}
		}

		if (wasBulletRemove == false)
		{
			for (int j = 1; j < _humans.size();)
			{
				if (_bullets[i].collideWithAgent(_humans[j]))
				{
					if (_humans[j]->applyDamage(_bullets[i].getDamage()))
					{
						delete _humans[j];

						_humans[j] = _humans.back();
						_humans.pop_back();
					}
					else
					{
						j++;
					}
					_bullets[i] = _bullets.back();
					_bullets.pop_back();

					i--;
					break;
				}
				else
				{
					j++;
				}
			}
		}
	}
}


void Game::checkVictory()
{
	if (_zombies.empty())
	{
		fatalError("You Win");
	}
}

void Game::processInput()
{
	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	bool quit = false;
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			state = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputMgr.setMouseCoords(e.motion.x, e.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputMgr.pressKey(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputMgr.releaseKey(e.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputMgr.pressKey(e.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputMgr.releaseKey(e.button.button);
			break;
		}
	}

}

void Game::drawGame()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	_colorProgram->use();
	glActiveTexture(GL_TEXTURE0);
	
	GLint texLocation = _colorProgram->getUniformLocation("mySampler");
	glUniform1i(texLocation, 0);

	GLuint pLocation = _colorProgram->getUniformLocation("P");
	glm::mat4 cameraMat = _camera->getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMat[0][0]));

	_spriteBatch.begin();

	glm::vec2 agDims(AGENT_RADIUS*2.0f);

	_levels[_currentLevel]->draw();
	for (int i = 0; i < _humans.size(); i++)
	{
		if (_camera->isBoxInView(_humans[i]->getPosition(), agDims))
		{
			_humans[i]->draw(_spriteBatch);
		}
		
	}
	for (int i = 0; i < _zombies.size(); i++)
	{
		if (_camera->isBoxInView(_zombies[i]->getPosition(), agDims))
		{
			_zombies[i]->draw(_spriteBatch);
		}
		
	}
	for (int i = 0; i < _bullets.size(); i++)
	{
		_bullets[i].draw(_spriteBatch);
	}

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	drawHud();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram->unuse();

	_window->swapBuffer();
}

void Game::exit()
{
}

void Game::drawHud()
{
	char buffer[256];

	GLuint pLocation = _colorProgram->getUniformLocation("P");
	glm::mat4 cameraMat = _hudCamera->getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMat[0][0]));

	_hudSpriteBatch.begin();

	sprintf(buffer, "Num Humans: %d", _humans.size());
	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(0,0),glm::vec2(0.5f), 0.0f, Zhu::Color(255,255,255,255));

	sprintf(buffer, "Num Zombies: %d", _zombies.size());
	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(0, 32), glm::vec2(0.5f), 0.0f, Zhu::Color(255, 255, 255, 255));

	_hudSpriteBatch.end();
	_hudSpriteBatch.renderBatch();
}