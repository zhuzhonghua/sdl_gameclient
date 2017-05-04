#include "stdafx.h"

#include "game.h"
#include "input.h"
#include "imageloader.h"

#include "engine.h"

#include "resourcemanager.h"

Game* Game::_inst;

Game::Game()
{
	state = GameState::PLAY;
	_time = 0;

	_camera = new Zhu::Camera2D();
	_camera->init(WIN_WIDTH, WIN_HEIGHT);

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

	_window = new Zhu::Window();
	_window->create("Game1", WIN_WIDTH, WIN_HEIGHT);

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

	initShaders();

	_spriteBatch.init();

	_fpsLimiter.init(60);

	initLevels();
}

void Game::initLevels()
{
	_levels.push_back(new Level("data/Zombie/Levels/level1.txt"));
	_currentLevel = 0;

	_player = new Player();
	_player->init(4.0f, _levels[_currentLevel]->getStartPlayerPos(), &_inputMgr);

	_humans.push_back(_player);
}

void Game::run()
{
	SDL_StartTextInput();	
	while (state == GameState::PLAY)
	{
		_fpsLimiter.begin();

		_time += 0.01;
		processInput();

		_camera->update();

		drawGame();
		
		_fps = _fpsLimiter.end();
	
	}
	SDL_StopTextInput();
}

void Game::updateAgents()
{
	for (int i = 0; i < _humans.size();i++)
	{
		_humans[i]->update();
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

	_levels[_currentLevel]->draw();
	for (int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->draw(_spriteBatch);
	}

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram->unuse();

	_window->swapBuffer();
}

void Game::exit()
{
}