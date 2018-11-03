#pragma once

#include <map>
#include <string>
#include "texture.h"

namespace Zengine{
class TextureCache
{
	static TextureCache* _inst;

	static TextureCache* inst();
	TextureCache();
public:
	~TextureCache();

	static Texture* getTexture(std::string);
	static Texture* createSolid(int color);

	static SDL_Surface* createSurface(int w, int h, int argb);

	static bool contains(const std::string& key);

	static void add(const std::string& key, Texture* tx)
	{
		inst()->_texMap.insert(std::make_pair(key, tx));
	}
private:
	std::map<std::string, Texture*> _texMap;
};
}