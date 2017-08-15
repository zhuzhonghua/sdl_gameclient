#include "stdafx.h"

#include "wndstory.h"
#include "simpleresource.h"
#include "pixelscene.h"
#include "game.h"
#include "dungeon.h"

const float WndStory::bgR = 0.77f;
const float WndStory::bgG = 0.73f;
const float WndStory::bgB = 0.62f;

std::map<int, std::string> WndStory::CHAPTERS;

namespace{
	class TouchAreaNew :public TouchArea{
	public:
		TouchAreaNew(WndStory* w, Visual* t) :wnd(w), TouchArea(t){}
	protected:
		WndStory* wnd;
		
		virtual void onClick(TouchScreen::Touch* touch) {
			wnd->hide();
			delete wnd;
		}
	};
}

WndStory::WndStory(const std::string& text)
:Window(0,0,Chrome::get(Chrome::SCROLL))
{
	if (CHAPTERS.size() <= 0)
	{
		CHAPTERS.insert(std::make_pair(ID_SEWERS,
			std::string("The Dungeon lies right beneath the City, its upper levels actually constitute the City's sewer system. ")+
			std::string("Being nominally a part of the City, these levels are not that dangerous. No one will call it a safe place, ")+
			std::string("but at least you won't need to deal with evil magic here.")));

		CHAPTERS.insert(std::make_pair(ID_PRISON,
			std::string("Many years ago an underground prison was built here for the most dangerous criminals. At the time it seemed ") +
			std::string("like a very clever idea, because this place indeed was very hard to escape. But soon dark miasma started to permeate ") +
			std::string("from below, driving prisoners and guards insane. In the end the prison was abandoned, though some convicts ") +
			std::string("were left locked up here.")));

		CHAPTERS.insert(std::make_pair(ID_CAVES,
			std::string("The caves, which stretch down under the abandoned prison, are sparcely populated. They lie too deep to be exploited ") +
			std::string("by the City and they are too poor in minerals to interest the dwarves. In the past there was a trade outpost ") +
			std::string("somewhere here on the route between these two states, but it has perished since the decline of Dwarven Metropolis. ") +
			std::string("Only omnipresent gnolls and subterranean animals dwell here now.")));

		CHAPTERS.insert(std::make_pair(ID_METROPOLIS,
			std::string("Dwarven Metropolis was once the greatest of dwarven city-states. In its heyday the mechanized army of dwarves ") +
			std::string("has successfully repelled the invasion of the old god and his demon army. But it is said, that the returning warriors ") +
			std::string("have brought seeds of corruption with them, and that victory was the beginning of the end for the underground kingdom.")));

		CHAPTERS.insert(std::make_pair(ID_HALLS,
			std::string("In the past these levels were the outskirts of Metropolis. After the costly victory in the war with the old god ") +
			std::string("dwarves were too weakened to clear them of remaining demons. Gradually demons have tightened their grip on this place ") +
			std::string("and now it's called Demon Halls.\n\n") +
			std::string("Very few adventurers have ever descended this far...")));
	}

	tf = PixelScene::createMultiline(text, 7);
	tf->maxWidth = WIDTH - MARGIN * 2;
	tf->measure();
	tf->ra = bgR;
	tf->ga = bgG;
	tf->ba = bgB;
	tf->rm = -bgR;
	tf->gm = -bgG;
	tf->bm = -bgB;
	tf->x = MARGIN;
	add(tf);

	add(new TouchAreaNew(this, chrome));

	resize((int)(tf->Width() + MARGIN * 2), (int)std::min((int)tf->Height(), 180));
}

void WndStory::update()
{
	Window::update();

	if (delay > 0 && (delay -= Game::elapsed) <= 0) 
	{
		shadow->visible = chrome->visible = tf->visible = true;
	}
}

void WndStory::showChapter(int id)
{
	if (Dungeon::chapters.find(id) != Dungeon::chapters.end()) 
	{
		return;
	}

	std::map<int, std::string>::iterator itr = CHAPTERS.find(id);
	if (itr != CHAPTERS.end())
	{
		WndStory* wnd = new WndStory(itr->second);
		if ((wnd->delay = 0.6f) > 0) {
			wnd->shadow->visible = wnd->chrome->visible = wnd->tf->visible = false;
		}

		Game::scene()->add(wnd);

		Dungeon::chapters.insert(id);
	}
}