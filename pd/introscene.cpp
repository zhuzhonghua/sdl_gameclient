#include "introscene.h"

#include "wndstory.h"
#include "game.h"
#include "interlevelscene.h"

const std::string IntroScene::TEXT = 
std::string("Many heroes of all kinds ventured into the Dungeon before you. Some of them have returned with treasures and magical ") +
std::string("artifacts, most have never been heard of since. But none have succeeded in retrieving the Amulet of Yendor, ") +
std::string("which is told to be hidden in the depths of the Dungeon.\n\n") +
std::string("") +
std::string("You consider yourself ready for the challenge, but most importantly, you feel that fortune smiles on you. ") +
std::string("It's time to start your own adventure!");;

namespace{
	class WndStoryNew :public WndStory{
	public:
		virtual void hide()
		{
			WndStory::hide();
			Game::switchScene(new InterLevelScene());
			delete this;
		}
	};
}
void IntroScene::init()
{
	PixelScene::init();
	add(new WndStory(TEXT));
	fadeIn();
}