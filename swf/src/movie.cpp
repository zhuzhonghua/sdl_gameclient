#include "movie.h"
#include "root.h"
#include "loader.h"
#include "tagloader.h"

#include <algorithm>

Movie::Movie(Root* r)
{
	_r = r;
	_curFrame = 0;
}

Movie::~Movie()
{

}

bool Movie::load(const char* name)
{
	_l = new Loader();
	_l->load(name);
	readHeader();
	readTags();
	return true;
}

SDL_Renderer* Movie::getRender()
{
	return _r->_renderer;
}

void Movie::addCharacter(int id, Character* ch)
{
	std::map<int, Character*>::iterator iter = _chs.find(id);
	Assert(iter == _chs.end());
	_chs.insert(std::make_pair(id, ch));
}

Character* Movie::getCharacter(int id)
{
	std::map<int, Character*>::iterator iter = _chs.find(id);
	if (iter != _chs.end())
	{
		return iter->second;
	}
	else
	{
		return NULL;
	}
}

void Movie::addControlTag(ControlTag* ct)
{
	_frames[_curFrame].push_back(ct);
}

void Movie::addDisplayChildAt(int depth, SWFDisplay* sd)
{
	std::vector<SWFDisplay*>::iterator itr = _playList.begin();
	while (itr != _playList.end())
	{
		if ((*itr)->depth > depth)
		{
			break;
		}
	}
	if (itr == _playList.end())
	{
		_playList.push_back(sd);
	}
	else
	{
		itr--;
		_playList.insert(itr, sd);
	}
	std::sort(_playList.begin(), _playList.end());
}

SWFDisplay* Movie::getDisplayChildAt(int depth)
{
	std::vector<SWFDisplay*>::iterator itr = _playList.begin();
	while (itr != _playList.end())
	{
		if ((*itr)->depth == depth)
		{
			return (*itr);
		}
	}
	return NULL;
}

void Movie::removeDisplayChildAt(int depth)
{
	std::vector<SWFDisplay*>::iterator itr = _playList.begin();
	while (itr != _playList.end())
	{
		if ((*itr)->depth == depth)
		{
			delete *itr;
			_playList.erase(itr);
			break;
		}
	}
}

void Movie::readHeader()
{
	UInt32 header = _l->readU32();
	UInt32 length = _l->readU32();

	_version = (header >> 24) & 255;
	if ((header & 0x0FFFFFF) != 0x00535746
		&& (header & 0x0FFFFFF) != 0x00535743)
	{
		Assert(0);
	}

	_l->swfVersion(_version);
	
	bool	compressed = (header & 255) == 'C';
	_l->setCompressed(compressed);

	_frame_size.read(_l);
	_frame_rate = _l->readU16() / 256.0f;

	_frame_count = _l->readU16();
	_frame_count = Max(_frame_count, 1);

	_frames.resize(_frame_count);
}

void Movie::readTags()
{
	bool end = false;
	while (!end)
	{
		_l->align();

		TagHeader th = _l->openTag();

		switch ((SWFTAG::TAG)th.tagType)
		{
		case SWFTAG::END:
			end = true;
			break;
		case SWFTAG::FILEATTRIBUTE:
			break;
		case SWFTAG::SETBACKGROUNDCOLOR:
			_bg.read_rgb(_l);
			break;
		case SWFTAG::DEFINEBITSJPEG2:
			TagLoader::loadDefineBitsJPEG(this, &th);
			break;
		case SWFTAG::DEFINESHAPE:
		case SWFTAG::DEFINESHAPE2:
		case SWFTAG::DEFINESHAPE3:
			TagLoader::loadDefineShape(this, &th);
			break;
		case SWFTAG::PLACEOBJECT2:
			TagLoader::loadPlaceObject(this, &th);
			break;
		case SWFTAG::SHOWFRAME :
			incCurFrame();
			break;
		}

		_l->closeTag();
	}
}

void Movie::play()
{
	_r->addPlayList(this);
	_curFrame = 0;
}

void Movie::advance()
{
	if (_curFrame < _frame_count)
	{
		std::vector<ControlTag*>& cts = _frames[_curFrame];
		std::vector<ControlTag*>::iterator itr = cts.begin();
		for (; itr != cts.end(); ++itr)
		{
			(*itr)->execute(this);
		}

		_curFrame++;
	}
}

void Movie::update()
{
	SDL_SetRenderDrawColor(_r->_renderer, _bg._r, _bg._g, _bg._b, _bg._a);
	SDL_Rect size;
	_frame_size.copy(&size);
	SDL_RenderFillRect(_r->_renderer, &size);

	std::vector<SWFDisplay*>::iterator itr = _playList.begin();
	for (; itr != _playList.end(); ++itr)
	{
		(*itr)->update(this);
	}
}