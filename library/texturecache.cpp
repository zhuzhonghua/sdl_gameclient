#include "texturecache.h"
#include "imageloader.h"
using namespace Zhu;
TextureCache::TextureCache()
{

}

TextureCache::~TextureCache()
{

}

Texture TextureCache::getTexture(std::string texPath)
{
	std::map<std::string, Texture>::iterator mit = _texMap.find(texPath);
	if (mit == _texMap.end())
	{
		Texture newTex = ImageLoader::loadPNG(texPath);
		_texMap.insert(std::make_pair(texPath, newTex));
		return newTex;
	}

	return mit->second;
}