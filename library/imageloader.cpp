#include "imageloader.h"
#include "stdafx.h"
#include "picoPNG.h"
#include "iomanager.h"

using namespace Zhu;
Texture ImageLoader::loadPNG(std::string filePath)
{
	return picoLoadPNG(filePath);
	//return sdlLoadPNG(filePath);
}

Texture ImageLoader::picoLoadPNG(std::string filePath)
{
	Texture tex = {};

	//This is the input data to decodePNG, which we load from a file
	std::vector<unsigned char> in;
	//This is the output data from decodePNG, which is the pixel data for our texture
	std::vector<unsigned char> out;

	unsigned long width, height;

	//Read in the image file contents into a buffer
	if (IOManager::readFileToBuffer(filePath, in) == false) {
		fatalError("Failed to load PNG file to buffer!");
	}

	//Decode the .png format into an array of pixels
	int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
	if (errorCode != 0) {
		fatalError("decodePNG failed with error: " + std::to_string(errorCode));
	}

	//Generate the openGL texture object
	glGenTextures(1, &(tex.id));

	//Bind the texture object
	glBindTexture(GL_TEXTURE_2D, tex.id);
	//Upload the pixels to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

	//Set some texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//Generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	tex.width = width;
	tex.height = height;
	//tex.filePath = filePath;

	return tex;
}

Texture ImageLoader::sdlLoadPNG(std::string filePath)
{
	Texture tex = {};
	SDL_Surface* img = IMG_Load(filePath.c_str());
	if (img == NULL)
	{
		fatalError("Error Load Image " + filePath);
	}
	tex.width = img->w;	tex.height = img->h;	glGenTextures(1, &tex.id);
	int mode = GL_RGB;
	if (img->format->BytesPerPixel == 4)
	{
		mode = GL_RGBA;
	}
	glBindTexture(GL_TEXTURE_2D, tex.id);	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, mode, GL_UNSIGNED_BYTE, img->pixels);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(img);
	return tex;
}