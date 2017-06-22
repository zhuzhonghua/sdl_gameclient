#include "game.h"

#include "engine.h"
#include "resourcemanager.h"

#include <algorithm>
#include <random>
#include <time.h>

Game::Game()
{
	state = GameState::PLAY;

}

void Game::init()
{
	Zhu::init();

	_window = new Zhu::Window();
	_window->create("NinJa", WIN_WIDTH, WIN_HEIGHT);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	_camera = new Zhu::Camera2D();
	_camera->init(_window->getWidth(), _window->getHeight());
	//_camera->setPosition(glm::vec2(WIN_WIDTH / 2, WIN_HEIGHT / 2));
	_camera->setScale(32.0f);

	_hudCamera = new Zhu::Camera2D();
	_hudCamera->init(WIN_WIDTH, WIN_HEIGHT);
	_hudCamera->setPosition(glm::vec2(WIN_WIDTH / 2, WIN_HEIGHT / 2));

	_spriteFont = new Zhu::SpriteFont("data/Fonts/chintzy.ttf", 40);

	_spriteBatch.init();
	_hudSpriteBatch.init();

	_fpsLimiter.setMaxFPS(60.0f);

	_colorProgram = new Zhu::GLSLProgram();
	_colorProgram->compileShaders("data/ninja/textureShading.vert", "data/ninja/textureShading.frag");
	_colorProgram->addAttribute("vertexPosition");
	_colorProgram->addAttribute("vertexColor");
	_colorProgram->addAttribute("vertexUV");
	_colorProgram->linkShaders();

	//
	_tex = Zhu::ResourceManager::getTexture("data/ninja/bricks_top.png");

	b2Vec2 gravity(0.0f, -10.0f);
	_world = new b2World(gravity);
	
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -20.0f);
	b2Body* groundBody = _world->CreateBody(&groundBodyDef);
	
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	
	groundBody->CreateFixture(&groundBox, 0.0f);

	std::mt19937 randEngine;
	std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yPos(-15.0f, 15.0f);
	std::uniform_real_distribution<float> size(0.5f, 2.5f);
	std::uniform_int_distribution<int> color(0, 255);

	const int NUM_BOXES = 50;
	for (int i = 0; i < NUM_BOXES; i++)
	{
		Zhu::Color randColor;
		randColor.r = color(randEngine);
		randColor.g = color(randEngine);
		randColor.b = color(randEngine);
		randColor.a = 255;
	
		Box newBox;
		newBox.init(_world, glm::vec2(xPos(randEngine), yPos(randEngine)), glm::vec2(size(randEngine), size(randEngine)), _tex, randColor, false);
		_boxes.push_back(newBox);
	}

	_player.init(_world, glm::vec2(0.0f, 20.0f), glm::vec2(1.0f, 2.0f), Zhu::Color(255,255,255,255), true);

	_debugRenderer.init();
}

void Game::run()
{
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / 60;
	const int MAX_PHYSICS_STEPS = 6;
	const float MAX_DELTA_TIME = 1.0f;

	int previousTick = SDL_GetTicks();

	SDL_StartTextInput();
	while (state == GameState::PLAY)
	{
		_fpsLimiter.begin();

		float frameTime = SDL_GetTicks() - previousTick;
		previousTick = SDL_GetTicks();
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		_inputMgr.update();
		processInput();

		_player.update(&_inputMgr);

		int i = 0;
		while (totalDeltaTime > 0.0f && i++ < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			totalDeltaTime -= deltaTime;
		
			update(deltaTime);
		}
		
		_camera->update();
		drawGame();

		_fps = _fpsLimiter.end();
	}
	SDL_StopTextInput();
}

void Game::update(float deltaTime)
{
	_world->Step(1.0f / 60.0f, 6, 2);
}

void Game::exit()
{

}

void Game::drawGame()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	_colorProgram->use();

	GLint textureUniform = _colorProgram->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	glm::mat4 cameraMat = _camera->getCameraMatrix();
	GLuint pLocation = _colorProgram->getUniformLocation("P");
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMat[0][0]));	

	_spriteBatch.begin();
	for (int i = 0; i < _boxes.size(); i++)
	{
		Box& b = _boxes[i];
		b.draw(&_spriteBatch);
	}
	_player.draw(&_spriteBatch);

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	// RenderBox
	for (int i = 0; i < _boxes.size(); i++)
	{
		Box& b = _boxes[i];
		//glm::vec4 destRect;
		//destRect.x = b.getBody()->GetPosition().x - b.getDimen().x / 2.0f;
		//destRect.y = b.getBody()->GetPosition().y - b.getDimen().y / 2.0f;
		//destRect.z = b.getDimen().x;
		//destRect.w = b.getDimen().y;
		//
		//_debugRenderer.drawBox(destRect, Zhu::Color(255, 255, 255, 255), b.getBody()->GetAngle());

		_debugRenderer.drawCircle(glm::vec2(b.getBody()->GetPosition().x, b.getBody()->GetPosition().y),
									Zhu::Color(255, 255, 255, 255),
									std::max(b.getDimen().x / 2.0f, b.getDimen().y/2.0f));
	}
	glm::vec4 destRect;
	destRect.x = _player.getBox().getBody()->GetPosition().x - _player.getBox().getDimen().x / 2.0f;
	destRect.y = _player.getBox().getBody()->GetPosition().y - _player.getBox().getDimen().y / 2.0f;
	destRect.z = _player.getBox().getDimen().x;
	destRect.w = _player.getBox().getDimen().y;

	_debugRenderer.drawBox(destRect, Zhu::Color(255, 255, 255, 255), _player.getBox().getBody()->GetAngle());

	_debugRenderer.end();
	_debugRenderer.render(cameraMat, 2.0f);


	drawHUD();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram->unuse();

	_window->swapBuffer();
}

void Game::drawHUD()
{
	GLint textureUniform = _colorProgram->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 cameraMat2 = _hudCamera->getCameraMatrix();
	GLuint pLocation = _colorProgram->getUniformLocation("P");
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMat2[0][0]));

	const Zhu::Color fontColor(255, 0, 0, 255);
	char buffer[64];
	sprintf(buffer, "%.1f", _fps);

	//
	_hudSpriteBatch.begin();
	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(5.0f, WIN_HEIGHT - 40.0f), glm::vec2(1.0f), 0.0f, fontColor);
	_hudSpriteBatch.end();
	_hudSpriteBatch.renderBatch();
}

void Game::processInput()
{
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

	if (_inputMgr.isKeyPressed(SDLK_ESCAPE))
	{
		state = GameState::EXIT;
	}
}