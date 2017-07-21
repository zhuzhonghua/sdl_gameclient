#pragma once

#include <map>
#include <string>
#include "smarttexture.h"

class TextureCache
{
	static TextureCache* _inst;
	
	static TextureCache* inst();
	TextureCache();
public:	
	~TextureCache();

	static SmartTexture* getTexture(std::string);
	static SmartTexture* createSolid(int color);
private:
	std::map<std::string, SmartTexture*> _texMap;
};
