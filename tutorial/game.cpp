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


	initShaders();

	_spriteBatch.init();

	_fpsLimiter.init(60);
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

		//for (int i = 0; i < _bullets.size();)
		//{
		//	if (_bullets[i].update())
		//	{
		//		_bullets[i] = _bullets.back();
		//		_bullets.pop_back();
		//	}
		//	else
		//	{
		//		i++;
		//	}
		//}
		drawGame();
		
		_fps = _fpsLimiter.end();
		
		//static int frameCount = 0;
		//if (frameCount++ > 10000)
		//{
		//	std::cout << _fps << std::endl;
		//	frameCount = 0;
		//}
	}
	SDL_StopTextInput();
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

	//if (_inputMgr.isKeyPressed(SDLK_w))
	//{
	//	_camera->setPosition(_camera->getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	//}
	//	
	//if (_inputMgr.isKeyPressed(SDLK_s))
	//{
	//	_camera->setPosition(_camera->getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	//}
	//	
	//if (_inputMgr.isKeyPressed(SDLK_a))
	//{
	//	_camera->setPosition(_camera->getPosition() + glm::vec2(-CAMERA_SPEED, 0));
	//}
	//	
	//if (_inputMgr.isKeyPressed(SDLK_d))
	//{
	//	_camera->setPosition(_camera->getPosition() + glm::vec2(CAMERA_SPEED, 0));
	//}
	//	
	//if (_inputMgr.isKeyPressed(SDLK_q))
	//{
	//	_camera->setScale(_camera->getScale() + SCALE_SPEED);
	//}
	//	
	//if (_inputMgr.isKeyPressed(SDLK_e))
	//	_camera->setScale(_camera->getScale() - SCALE_SPEED);
	//
	//if (_inputMgr.isKeyPressed(SDL_BUTTON_LEFT))
	//{
	//	glm::vec2 mouseCoords = _inputMgr.getMouseCoords();
	//	mouseCoords = _camera->convertScreenToWorld(mouseCoords);
	//	//std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;
	//
	//	glm::vec2 playerPosition(0);
	//
	//	glm::vec2 direction = mouseCoords - playerPosition;
	//	direction = glm::normalize(direction);
	//	_bullets.emplace_back(glm::vec2(0), direction, 5.0f, 1000);
	//}
}

void Game::drawGame()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	_colorProgram->use();
	glActiveTexture(GL_TEXTURE0);
	
	GLint texLocation = _colorProgram->getUniformLocation("mySampler");
	glUniform1i(texLocation, 0);

	//GLuint timeLocation = _colorProgram->getUniformLocation("time");
	//glUniform1f(timeLocation, _time);

	GLuint pLocation = _colorProgram->getUniformLocation("P");
	glm::mat4 cameraMat = _camera->getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMat[0][0]));

	_spriteBatch.begin();

	//glm::vec4 pos(0.0f,0.0f,50.0f,50.0f);
	//glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	//
	//static Zhu::Texture tex = Zhu::ResourceManager::getTexture("data/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	//Zhu::Color color;
	//color.r = 255;
	//color.g = 255;
	//color.b = 255;
	//color.a = 255;
	//
	//_spriteBatch.draw(pos, uv, tex.id, 0.0f, color);
	//
	//for (int i = 0; i < _bullets.size(); i++)
	//{
	//	_bullets[i].draw(_spriteBatch);
	//}

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram->unuse();

	_window->swapBuffer();
}

void Game::exit()
{
}