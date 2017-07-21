#include "stdafx.h"

#include "util.h"

#include "game.h"

Random* Random::_inst;

Random::Random():
randomEngine(time(NULL))
{
}

Random* Random::inst()
{
	if (_inst == NULL)
	{
		_inst = new Random();
	}

	return _inst;
}

float Random::Float(float begin, float end)
{
	std::uniform_real_distribution<float> rand(begin, end);
	return rand(inst()->randomEngine);
}

int Random::Int(int begin, int end)
{
	std::uniform_int_distribution<int> rand(begin, end);
	return rand(inst()->randomEngine);
}

float GameMath::speed(float speed, float acc)
{
	if (acc != 0) 
	{
		speed += acc * Game::elapsed;
	}

	return speed;
}

float GameMath::gate(float min, float value, float max) 
{
	if (value < min) 
	{
		return min;
	}
	else if (value > max) 
	{
		return max;
	}
	else 
	{
		return value;
	}
}

void GameMath::MATRIXRotate(Matrix& mat, float angle)
{
	angle = angle * 3.14f / 180.0f;
	float s = sin(angle);
	float c = cos(angle);

	float m0 = mat[0][0];
	float m1 = mat[0][1];
	float m4 = mat[1][0];
	float m5 = mat[1][1];

	mat[0][0] = m0 * c + m4 * s;
	mat[0][1] = m1 * c + m5 * s;
	mat[1][0] = -m0 * s + m4 * c;
	mat[1][1] = -m1 * s + m5 * c;
}

void GameMath::MATRIXScale(Matrix& m, float x, float y)
{
	m[0][0] *= x;
	m[0][1] *= x;
	m[0][2] *= x;
	m[0][3] *= x;
	m[1][0] *= y;
	m[1][1] *= y;
	m[1][2] *= y;
	m[1][3] *= y;
}
// left = x
// top = y
// right = z
// bottom = w
float GameMath::RECTFWidth(const RectF& frame)
{
	return std::abs(frame.z-frame.x);
}

float GameMath::RECTFHeight(const RectF& frame)
{
	return std::abs(frame.w - frame.y);
}

RectF GameMath::RECTF(float left, float top, float right, float bottom)
{
	return RectF(
		(float)left,
		(float)top,
		(float)right,
		(float)bottom);
}

float GameMath::RECTFLeft(const RectF& frame)
{
	return frame.x;
}
float GameMath::RECTFTop(const RectF& frame)
{
	return frame.y;
}
float GameMath::RECTFRight(const RectF& frame)
{
	return frame.z;
}
float GameMath::RECTFBottom(const RectF& frame)
{
	return frame.w;
}

PointF* GameMath::PointFSet(PointF* p, float v)
{
	p->x = v;
	p->y = v;
	return p;
}

PointF* GameMath::PointFSet(PointF* p, float x, float y)
{
	p->x = x;
	p->y = y;
	return p;
}

int ColorMath::interpolate(int A, int B, float p)
{
	if (p <= 0)
	{
		return A;
	}
	else if (p >= 1)
	{
		return B;
	}

	int ra = A >> 16;
	int ga = (A >> 8) & 0xFF;
	int ba = A & 0xFF;

	int rb = B >> 16;
	int gb = (B >> 8) & 0xFF;
	int bb = B & 0xFF;

	float p1 = 1 - p;

	int r = (int)(p1 * ra + p * rb);
	int g = (int)(p1 * ga + p * gb);
	int b = (int)(p1 * ba + p * bb);

	return (r << 16) + (g << 8) + b;
}

int ColorMath::interpolate(float p, const std::vector<int>& colors)
{
	if (p <= 0) 
	{
		return colors[0];
	}
	else if (p >= 1) 
	{
		return colors[colors.size() - 1];
	}
	int segment = (int)(colors.size() * p);
	return interpolate(colors[segment], colors[segment + 1], (int)(p * (colors.size() - 1)) % 1);
}

int ColorMath::random(int a, int b)
{
	return interpolate(a, b, Random::Float(0, 1));
}

bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char> &buffer)
{
	SDL_RWops* fileOP = SDL_RWFromFile(filePath.c_str(), "r");
	if (fileOP == NULL)
	{
		perror(filePath.c_str());
		return false;
	}

	int len = SDL_RWsize(fileOP);

	buffer.resize(len);
	SDL_RWread(fileOP, &buffer[0], len, 1);
	SDL_RWclose(fileOP);

	return true;
}

bool IOManager::readFileToBuffer(std::string filePath, std::string &buffer)
{
	SDL_RWops* fileOP = SDL_RWFromFile(filePath.c_str(), "r");
	if (fileOP == NULL)
	{
		perror(filePath.c_str());
		return false;
	}

	int len = SDL_RWsize(fileOP);

	buffer.resize(len);
	SDL_RWread(fileOP, &buffer[0], len, 1);
	SDL_RWclose(fileOP);

	return true;
}