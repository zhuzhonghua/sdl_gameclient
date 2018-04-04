#include "tagloader.h"
#include "movie.h"
#include "loader.h"
#include "character.h"
#include "controltag.h"

#include <vector>
#include <SDL.h>
#include "SDL_image.h"

void TagLoader::loadDefineBitsJPEG(Movie* m, const TagHeader* th)
{
	if (th->tagType == SWFTAG::DEFINEBITSJPEG2)
	{
		int id = m->getLoader()->readU16();

		int size = m->getLoader()->getTagLeftSize();
		char* mem = new char[size];
		memset(mem, 0, size);
		m->getLoader()->readBytes(mem, size);
		SDL_RWops* op = SDL_RWFromMem(mem, size);
		SDL_Texture*tex = IMG_LoadTexture_RW(m->getRender(), op, 1);

		//SDL_Rect tex_rect;
		//tex_rect.x = 10;
		//tex_rect.y = 10;
		//tex_rect.w = 550;
		//tex_rect.h = 400;
		//SDL_RenderCopy(m->getRender(), tex, NULL, &tex_rect);
		//SDL_RenderPresent(m->getRender());
		//SDL_Delay(5000);

		Character* ch = new Character();
		ch->_tex = tex;
		ch->_type = th->tagType;
		m->addCharacter(id, ch);

		delete mem;
	}	
}

void TagLoader::loadDefineShape(Movie* m, const TagHeader* th)
{
	if (th->tagType == SWFTAG::DEFINESHAPE || 
		th->tagType == SWFTAG::DEFINESHAPE2 ||
		th->tagType == SWFTAG::DEFINESHAPE3)
	{
		Character* ch = new Character();
		ch->_type = th->tagType;
		ch->_id = m->getLoader()->readU16();
		ch->_bounds.read(m->getLoader());		

		ch->_shape = new ShapeWithStyle();
		ch->_shape->type = th->tagType;
		ch->_shape->read(m->getLoader());

		m->addCharacter(ch->_id, ch);
	}
}

void TagLoader::loadPlaceObject(Movie* m, const TagHeader* th)
{
	Loader* in = m->getLoader();
	if (th->tagType == SWFTAG::PLACEOBJECT2)
	{
		PlaceObject2Tag* pot = new PlaceObject2Tag();
		pot->read(in);

		m->addControlTag(pot);
	}
}
