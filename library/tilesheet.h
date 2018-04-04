#pragma once

#include "texture.h"
#include "glm/glm.hpp"

namespace Zhu{
class TileSheet{
public:
	void init(const Texture& tex, const glm::ivec2& tileDims)
	{
		this->tex = tex;
		this->dims = tileDims;
	}

	glm::vec4 getUVs(int idx)
	{
		int xTile = idx % dims.x;
		int yTile = idx / dims.x;

		glm::vec4 uv;
		uv.x = xTile / (float)dims.x;
		uv.y = yTile / (float)dims.y;
		uv.z = 1.0f / dims.x;
		uv.w = 1.0f / dims.y;
		return uv;
	}

	Texture tex;
	glm::ivec2 dims;
};
}