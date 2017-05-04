#include "resourcemanager.h"
using namespace Zhu;
TextureCache ResourceManager::_texCache;

Texture ResourceManager::getTexture(std::string texPath)
{
	return _texCache.getTexture(texPath);
}