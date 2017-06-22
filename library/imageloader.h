#pragma once

#include <string>

#include "texture.h"

namespace Zhu{
	class ImageLoader
	{
	public:
		static Texture loadPNG(std::string filePath);
	private:
		static Texture picoLoadPNG(std::string filePath);
		static Texture sdlLoadPNG(std::string filePath);
	};
}