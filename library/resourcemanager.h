#pragma once

#include "texturecache.h"

namespace Zhu{
	class ResourceManager
	{
	public:
		static Texture getTexture(std::string texPath);
	private:
		static TextureCache _texCache;
	};
}