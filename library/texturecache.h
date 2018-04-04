#pragma once

#include <map>
#include <string>
#include "texture.h"

namespace Zhu{
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		Texture getTexture(std::string);
	private:
		std::map<std::string, Texture> _texMap;
	};
}