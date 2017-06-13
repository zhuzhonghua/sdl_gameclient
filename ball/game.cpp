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
	_window->create("Ball", WIN_WIDTH, WIN_HEIGHT);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	_camera = new Zhu::Camera2D();
	_camera->init(WIN_WIDTH, WIN_HEIGHT);
	_camera->setPosition(glm::vec2(WIN_WIDTH / 2, WIN_HEIGHT / 2));

	_hudCamera = new Zhu::Camera2D();
	_hudCamera->init(WIN_WIDTH, WIN_HEIGHT);
	_hudCamera->setPosition(glm::vec2(WIN_WIDTH / 2, WIN_HEIGHT / 2));

	_spriteFont = new Zhu::SpriteFont("data/Fonts/chintzy.ttf", 40);

	_spriteBatch.init();
	_hudSpriteBatch.init();

	_fpsLimiter.setMaxFPS(60.0f);

	_colorProgram = new Zhu::GLSLProgram();
	_colorProgram->compileShaders("data/Ball/Shaders/textureShading.vert", "data/Ball/Shaders/textureShading.frag");
	_colorProgram->addAttribute("vertexPosition");
	_colorProgram->addAttribute("vertexColor");
	_colorProgram->addAttribute("vertexUV");
	_colorProgram->linkShaders();

	initRenderers();

	_grid = new Grid(WIN_WIDTH, WIN_HEIGHT, 128);
	initBalls();
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
	_ballCtl.updateBalls(_balls, _grid, deltaTime, WIN_WIDTH, WIN_HEIGHT);
}

void Game::exit()
{

}

struct BallSpawn{
	BallSpawn(const Zhu::Color& Color,
				float Radius, float Mass,float Minspeed,
				float Maxspeed, float Prob):
				color(Color),
				radius(Radius),
				mass(Mass),
				randSpeed(Minspeed,Maxspeed),
				prob(Prob){}

	Zhu::Color color;
	float radius;
	float mass;
	float prob;
	std::uniform_real_distribution<float> randSpeed;
};
void Game::initBalls()
{
	const int NUM_BALLS = 100;

	_balls.reserve(NUM_BALLS);

	std::mt19937 randEngine(time(NULL));
	std::uniform_real_distribution<float> randX(0.0f, WIN_WIDTH);
	std::uniform_real_distribution<float> randY(0.0f, WIN_HEIGHT);
	std::uniform_real_distribution<float> randDir(-1.0f,1.0f);

	std::vector<BallSpawn> possibleBalls;
	float totalProb = 0.0f;

	totalProb += 20.0f;
	possibleBalls.emplace_back(Zhu::Color(255,255,255,255),
								20.0f,1.0f,0.1f,7.0f,totalProb);

	totalProb += 10.0f;
	possibleBalls.emplace_back(Zhu::Color(0, 0, 255, 255),
								30.0f, 2.0f, 0.1f, 3.0f, totalProb);

	totalProb += 1.0f;
	possibleBalls.emplace_back(Zhu::Color(255, 0, 0, 255),
								50.0f, 4.0f, 0.0f, 0.0f, totalProb);

	std::uniform_real_distribution<float> spawn(0.0f, totalProb);

	BallSpawn* ballToSpawn = &possibleBalls[0];
	for (int i = 0; i < NUM_BALLS; i++)
	{
		float spawnVal = spawn(randEngine);
		for (int j = 0; j < possibleBalls.size(); j++)
		{
			if (spawnVal <= possibleBalls[j].prob)
			{
				ballToSpawn = &possibleBalls[j];
				break;
			}
		}

		glm::vec2 pos(randX(randEngine), randY(randEngine));

		glm::vec2 dir(randDir(randEngine), randDir(randEngine));
		if (dir.x != 0.0f || dir.y != 0.0f)
		{
			dir = glm::normalize(dir);
		}
		else
		{
			dir = glm::vec2(1.0f, 0.0f);
		}

		_balls.emplace_back(ballToSpawn->radius, ballToSpawn->mass, pos, dir*ballToSpawn->randSpeed(randEngine), Zhu::ResourceManager::getTexture("data/Ball/circle.png").id, ballToSpawn->color);
		_grid->addBall(&_balls.back());
	}
}

void Game::initRenderers()
{
	_currentRenderer = 0;
	_ballRenderers.push_back(new BallRenderer());	
}

void Game::drawGame()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);

	_colorProgram->use();

	GLint textureUniform = _colorProgram->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 cameraMat = _camera->getCameraMatrix();
	GLuint pLocation = _colorProgram->getUniformLocation("P");
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMat[0][0]));	

	_ballRenderers[_currentRenderer]->renderBalls(_spriteBatch, _balls, cameraMat);

	drawHUD();

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
			_ballCtl.onMouseMove(_balls, (float)e.motion.x, (float)WIN_HEIGHT - (float)e.motion.y);
			_inputMgr.setMouseCoords(e.motion.x, e.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputMgr.pressKey(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputMgr.releaseKey(e.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_ballCtl.onMouseDown(_balls, (float)e.motion.x, (float)WIN_HEIGHT - (float)e.motion.y);
			_inputMgr.pressKey(e.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_ballCtl.onMouseUp(_balls);
			_inputMgr.releaseKey(e.button.button);
			break;
		}
	}

	if (_inputMgr.isKeyPressed(SDLK_ESCAPE))
	{
		state = GameState::EXIT;
	}

	if (_inputMgr.isKeyPressed(SDLK_LEFT))
	{
		_ballCtl.setGravityDirection(GravityDirection::LEFT);
	}
	else if (_inputMgr.isKeyPressed(SDLK_RIGHT))
	{
		_ballCtl.setGravityDirection(GravityDirection::RIGHT);
	}
	else if (_inputMgr.isKeyPressed(SDLK_UP))
	{
		_ballCtl.setGravityDirection(GravityDirection::UP);
	}
	else if (_inputMgr.isKeyPressed(SDLK_DOWN))
	{
		_ballCtl.setGravityDirection(GravityDirection::DOWN);
	}
	else if (_inputMgr.isKeyPressed(SDLK_SPACE))
	{
		_ballCtl.setGravityDirection(GravityDirection::NONE);
	}

	if (_inputMgr.isKeyPressed(SDLK_1)) {
		_currentRenderer++;
		if (_currentRenderer >= (int)_ballRenderers.size()) {
			_currentRenderer = 0;
		}
	}
}