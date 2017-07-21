#include "stdafx.h"
#include "texture.h"

#include "util.h"
#include "picoPNG.h"

Texture::Texture()
{
	premultiplied = false;

	glGenTextures(1, &id);
	bind();
}

Texture::~Texture()
{
	// TODO: MemoryLeak
	//destroy();
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::activate(int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
}

void Texture::filter(int minMode, int maxMode)
{
	bind();

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMode);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, maxMode);
}

void Texture::wrap(int s, int t)
{
	bind();

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}

void Texture::destroy()
{
	glDeleteTextures(1, &id);
}

void Texture::pixels(int w, int h, std::vector<byte> data)
{
	bind();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(data[0]));
}

Texture* Texture::create(int width, int height, std::vector<byte> data)
{
	Texture* tex = new Texture();
	tex->pixels(width, height, data);
	return tex;
}

void Texture::bitmap(const std::string& bitmap)
{
	//_getBytesPNG(bitmap, out, w, h);
	SDL_Surface* img = IMG_Load(bitmap.c_str());
	//width = w, height = h;
	width = img->w;
	height = img->h;

	bind();

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

	SDL_FreeSurface(img);

	premultiplied = true;
}

void Texture::bitmap(SDL_Surface* img)
{
	//width = w, height = h;
	width = img->w;
	height = img->h;

	bind();

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

	premultiplied = true;
}

void Texture::_getBytesPNG(const std::string& bitmap, std::vector<byte>& data, unsigned long &width, unsigned long &height)
{
	////This is the input data to decodePNG, which we load from a file
	//std::vector<unsigned char> in;
	//
	////Read in the image file contents into a buffer
	//if (IOManager::readFileToBuffer(bitmap, in) == false)
	//{
	//	fatalError("Failed to load PNG file to buffer!");
	//}
	//
	////Decode the .png format into an array of pixels
	//int errorCode = decodePNG(data, width, height, &(in[0]), in.size());
	//if (errorCode != 0)
	//{
	//	fatalError("decodePNG failed with error: " + std::to_string(errorCode));
	//}
}