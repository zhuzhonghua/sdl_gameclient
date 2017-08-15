#include "interlevelscene.h"
#include "game.h"
#include "wnderror.h"
#include "startscene.h"
#include "dungeon.h"
#include "gamescene.h"


const float InterLevelScene::TIME_TO_FADE = 0.3f;

const std::string InterLevelScene::TXT_DESCENDING = "Descending...";
const std::string InterLevelScene::TXT_ASCENDING = "Ascending...";
const std::string InterLevelScene::TXT_LOADING = "Loading...";
const std::string InterLevelScene::TXT_RESURRECTING = "Resurrecting...";
const std::string InterLevelScene::TXT_RETURNING = "Returning...";
const std::string InterLevelScene::TXT_FALLING = "Falling...";

const std::string InterLevelScene::ERR_FILE_NOT_FOUND = "File not found. For some reason.";
const std::string InterLevelScene::ERR_GENERIC = "Something went wrong...";

InterLevelScene::Mode InterLevelScene::mode;

int InterLevelScene::returnDepth;
int InterLevelScene::returnPos;

bool InterLevelScene::noStory = false;

bool InterLevelScene::fallIntoPit;

namespace{
	void* InterlevelSceneThread(void* Param)
	{
		InterLevelScene* sce = (InterLevelScene*)Param;

		//try {
			//
			//Generator.reset();
			
		switch (sce->mode) 
		{
		case InterLevelScene::DESCEND:
			sce->descend();
			break;
		case InterLevelScene::ASCEND:
			sce->ascend();
			break;
		case InterLevelScene::CONTINUE:
			sce->restore();
			break;
		case InterLevelScene::RESURRECT:
			sce->resurrect();
			break;
		case InterLevelScene::RETURN:
			sce->returnTo();
			break;
		case InterLevelScene::FALL:
			sce->fall();
			break;
		}
			
		if ((Dungeon::depth % 5) == 0) 
		{
			//Sample.INSTANCE.load(Assets.SND_BOSS);
		}
			//
		//	}
		//	catch (FileNotFoundException e) {
		//	
		//		error = ERR_FILE_NOT_FOUND;
		//	
		//	}
		//	catch (Exception e) {
		//	
		//		error = ERR_GENERIC;
		//	
		//	}
			
		if (sce->phase == InterLevelScene::Phase::STATIC && sce->error.length() <= 0) 
		{
			sce->phase = InterLevelScene::Phase::FADE_OUT;
			sce->timeLeft = InterLevelScene::TIME_TO_FADE;
		}

		return NULL;
	}
}
void InterLevelScene::init()
{
	PixelScene::init();

	std::string text;
	switch (mode) 
	{
	case DESCEND:
		text = TXT_DESCENDING;
		break;
	case ASCEND:
		text = TXT_ASCENDING;
		break;
	case CONTINUE:
		text = TXT_LOADING;
		break;
	case RESURRECT:
		text = TXT_RESURRECTING;
		break;
	case RETURN:
		text = TXT_RETURNING;
		break;
	case FALL:
		text = TXT_FALLING;
		break;
	}

	message = PixelScene::createText(text, 9);
	message->measure();
	message->x = (Camera::mainCamera->width - message->Width()) / 2;
	message->y = (Camera::mainCamera->height - message->Height()) / 2;
	add(message);

	phase = Phase::FADE_IN;
	timeLeft = TIME_TO_FADE;

	pthread_create(&threadID, NULL, InterlevelSceneThread, this);
}

namespace{
	class WndErrorNew :public WndError{
	public:
		WndErrorNew(const std::string& msg)
			:WndError(msg)
		{}

		virtual void onBackPressed()
		{			
			Game::switchScene(new StartScene());
			WndError::onBackPressed();
		}
	};
}

bool InterLevelScene::isThreadAlive()
{
	int rc = pthread_kill(threadID, 0);
	if (rc == ESRCH)
	{
		return false;
	}
	else if (rc == EINVAL)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void InterLevelScene::update()
{
	PixelScene::update();

	float p = timeLeft / TIME_TO_FADE;

	switch (phase) {

	case FADE_IN:
		message->alpha(1 - p);
		if ((timeLeft -= Game::elapsed) <= 0) 
		{
			if (!isThreadAlive() && error.length() <= 0)
			{
				phase = Phase::FADE_OUT;
				timeLeft = TIME_TO_FADE;
			}
			else 
			{
				phase = Phase::STATIC;
			}
		}
		break;

	case FADE_OUT:
		message->alpha(p);
		if (mode == Mode::CONTINUE || (mode == Mode::DESCEND && Dungeon::depth == 1)) 
		{
			//Music.INSTANCE.volume(p);
		}
		if ((timeLeft -= Game::elapsed) <= 0) 
		{
			Game::switchScene(new GameScene());
		}
		break;

	case STATIC:
		if (error.length() > 0) 
		{
			add(new WndErrorNew(error));
			error.clear();
		}
		break;
	}
}

void InterLevelScene::descend()
{
	//Actor.fixTime();
	//if (Dungeon.hero == null) {
	//	Dungeon.init();
	//	if (noStory) {
	//		Dungeon.chapters.add(WndStory.ID_SEWERS);
	//		noStory = false;
	//	}
	//	GameLog.wipe();
	//}
	//else {
	//	Dungeon.saveLevel();
	//}
	//
	//Level level;
	//if (Dungeon.depth >= Statistics.deepestFloor) {
	//	level = Dungeon.newLevel();
	//}
	//else {
	//	Dungeon.depth++;
	//	level = Dungeon.loadLevel(Dungeon.hero.heroClass);
	//}
	//Dungeon.switchLevel(level, level.entrance);
}

void InterLevelScene::fall()
{
	//Actor.fixTime();
	//Dungeon.saveLevel();
	//
	//Level level;
	//if (Dungeon.depth >= Statistics.deepestFloor) {
	//	level = Dungeon.newLevel();
	//}
	//else {
	//	Dungeon.depth++;
	//	level = Dungeon.loadLevel(Dungeon.hero.heroClass);
	//}
	//Dungeon.switchLevel(level, fallIntoPit ? level.pitCell() : level.randomRespawnCell());
}

void InterLevelScene::ascend()
{
	//Actor.fixTime();
	//
	//Dungeon.saveLevel();
	//Dungeon.depth--;
	//Level level = Dungeon.loadLevel(Dungeon.hero.heroClass);
	//Dungeon.switchLevel(level, level.exit);
}

void InterLevelScene::returnTo()
{
	//Actor.fixTime();
	//
	//Dungeon.saveLevel();
	//Dungeon.depth = returnDepth;
	//Level level = Dungeon.loadLevel(Dungeon.hero.heroClass);
	//Dungeon.switchLevel(level, Level.resizingNeeded ? level.adjustPos(returnPos) : returnPos);
}

void InterLevelScene::restore()
{
	//Actor.fixTime();
	//
	//GameLog.wipe();
	//
	//Dungeon.loadGame(StartScene.curClass);
	//if (Dungeon.depth == -1) {
	//	Dungeon.depth = Statistics.deepestFloor;
	//	Dungeon.switchLevel(Dungeon.loadLevel(StartScene.curClass), -1);
	//}
	//else {
	//	Level level = Dungeon.loadLevel(StartScene.curClass);
	//	Dungeon.switchLevel(level, Level.resizingNeeded ? level.adjustPos(Dungeon.hero.pos) : Dungeon.hero.pos);
	//}
}

void InterLevelScene::resurrect()
{
	//Actor.fixTime();
	//
	//if (Dungeon.bossLevel()) {
	//	Dungeon.hero.resurrect(Dungeon.depth);
	//	Dungeon.depth--;
	//	Level level = Dungeon.newLevel();
	//	Dungeon.switchLevel(level, level.entrance);
	//}
	//else {
	//	Dungeon.hero.resurrect(-1);
	//	Dungeon.resetLevel();
	//}
}