#pragma once

namespace Zengine{
class Texture{
public:
	static const int NEAREST = GL_NEAREST;
	static const int LINEAR = GL_LINEAR;
	static const int REPEAT = GL_REPEAT;
	static const int MIRROR = GL_MIRRORED_REPEAT;
	static const int CLAMP = GL_CLAMP_TO_EDGE;

	int width;
	int height;

	GLuint id;
	bool premultiplied;

	SDL_Surface* texSrc;

	Texture();
	Texture(SDL_Surface* img);
	Texture(const std::string& bitmap);
	virtual ~Texture();

	void init();
	void bind();
	virtual void filter(int minMode, int maxMode);
	virtual void wrap(int s, int t);
	void destroy();

	virtual void bitmap(const std::string& bitmap);
	virtual void bitmap(SDL_Surface* img);

	void pixels(int w, int h, std::vector<byte> data);
	void pixels(int w, int h, std::vector<int> pixels);

	static Texture* create(int width, int height, std::vector<byte> data);
	static void activate(int index);

	int getPixel(int x, int y);
	void setPixel(int x, int y, int color);
};
}