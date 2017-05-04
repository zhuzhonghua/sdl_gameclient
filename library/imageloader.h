#pragma once

#include <string>

#include "texture.h"

namespace Zhu{
	class ImageLoader
	{
	public:
		static Texture loadPNG(std::string filePath);

	};
}