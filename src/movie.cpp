#include "movie.h"
#include "root.h"
#include "loader.h"

Movie::Movie(Root* r)
{
	_r = r;
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

	bool	compressed = (header & 255) == 'C';
	_l->setCompressed(compressed);

	_frame_size.read(_l);
	_frame_rate = _l->readU16() / 256.0f;

	set_frame_count(_l->readU16());
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
			break;
		case SWFTAG::DEFINESHAPE:
			break;
		case SWFTAG::PLACEOBJECT2:
			break;
		case SWFTAG::SHOWFRAME :
			break;
		}

		_l->closeTag();
	}
}

void Movie::play()
{
	_r->addPlayList(this);
}

void Movie::advance()
{}
void Movie::update()
{
	SDL_SetRenderDrawColor(_r->_renderer, _bg._r, _bg._g, _bg._b, _bg._a);
	SDL_Rect size;
	_frame_size.copy(&size);
	SDL_RenderFillRect(_r->_renderer, &size);
}