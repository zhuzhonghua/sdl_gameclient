#include "imageloader.h"
#include "stdafx.h"
using namespace Zhu;
Texture ImageLoader::loadPNG(std::string filePath)
{
	Texture tex = {};

	SDL_Surface* img = IMG_Load(filePath.c_str());
	if (img == NULL)
	{
		fatalError("Error Load Image "+filePath);
	}

	tex.width = img->w;
	tex.height = img->h;

	glGenTextures(1, &tex.id);
	glBindTexture(GL_TEXTURE_2D, tex.id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(img);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}