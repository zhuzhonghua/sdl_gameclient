#include "stdafx.h"
#include "util.h"
#include "game.h"
#include <stdarg.h>

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

float Random::Float(float end)
{
	std::uniform_real_distribution<float> rand(0.0f, end);
	return rand(inst()->randomEngine);
}

int Random::Int(int begin, int end)
{
	std::uniform_int_distribution<int> rand(begin, end);
	return rand(inst()->randomEngine);
}

int Random::Int(int end)
{
	std::uniform_int_distribution<int> rand(0, end);
	return rand(inst()->randomEngine);
}

const float GameMath::POINTF_PI = 3.1415926f;
const float GameMath::POINTF_PI2 = GameMath::POINTF_PI * 2;
const float GameMath::POINTF_G2R = GameMath::POINTF_PI / 180;

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

void GameMath::RECTFOffset(RectF& frame, float dx, float dy)
{
	frame.x += dx;
	frame.z += dx;

	frame.y += dy;
	frame.w += dy;
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

PointF* GameMath::PointFOffSet(PointF* p, PointF* dp)
{
	p->x += dp->x;
	p->y += dp->y;
	return p;
}

PointF* GameMath::PointFOffSet(PointF* p, float dx, float dy)
{
	p->x += dx;
	p->y += dy;
	return p;
}

PointF* GameMath::PointFPolar(PointF* p, float a, float l)
{
	p->x = l * cos(a);// FloatMath.cos(a);
	p->y = l * sin(a);// FloatMath.sin(a);
	return p;
}

PointF* GameMath::PointFScale(PointF* p, float f)
{
	p->x *= f;
	p->y *= f;
	return p;
}

void GameMath::splitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

void GameMath::splitStringRe(const std::string& s, std::vector<std::string>& v, pcrecpp::RE re)
{
	pcrecpp::StringPiece input(s);
	const char* lastP = input.data();

	std::string match;
	while (re.Consume(&input, &match))
	{
		const char* p = input.data();
		int range = p - lastP;
		v.push_back(std::string(lastP, lastP + range));

		lastP += range;
		lastP += match.length();
	}
	v.push_back(std::string(lastP));
}

/* Gets a unicode value from a UTF-8 encoded string and advance the string */
#define UNKNOWN_UNICODE 0xFFFD
static Uint32 UTF8_getch(const char **src, size_t *srclen)
{
	const Uint8 *p = *(const Uint8**)src;
	size_t left = 0;
	SDL_bool overlong = SDL_FALSE;
	SDL_bool underflow = SDL_FALSE;
	Uint32 ch = UNKNOWN_UNICODE;

	if (*srclen == 0) {
		return UNKNOWN_UNICODE;
	}
	if (p[0] >= 0xFC) {
		if ((p[0] & 0xFE) == 0xFC) {
			if (p[0] == 0xFC && (p[1] & 0xFC) == 0x80) {
				overlong = SDL_TRUE;
			}
			ch = (Uint32)(p[0] & 0x01);
			left = 5;
		}
	}
	else if (p[0] >= 0xF8) {
		if ((p[0] & 0xFC) == 0xF8) {
			if (p[0] == 0xF8 && (p[1] & 0xF8) == 0x80) {
				overlong = SDL_TRUE;
			}
			ch = (Uint32)(p[0] & 0x03);
			left = 4;
		}
	}
	else if (p[0] >= 0xF0) {
		if ((p[0] & 0xF8) == 0xF0) {
			if (p[0] == 0xF0 && (p[1] & 0xF0) == 0x80) {
				overlong = SDL_TRUE;
			}
			ch = (Uint32)(p[0] & 0x07);
			left = 3;
		}
	}
	else if (p[0] >= 0xE0) {
		if ((p[0] & 0xF0) == 0xE0) {
			if (p[0] == 0xE0 && (p[1] & 0xE0) == 0x80) {
				overlong = SDL_TRUE;
			}
			ch = (Uint32)(p[0] & 0x0F);
			left = 2;
		}
	}
	else if (p[0] >= 0xC0) {
		if ((p[0] & 0xE0) == 0xC0) {
			if ((p[0] & 0xDE) == 0xC0) {
				overlong = SDL_TRUE;
			}
			ch = (Uint32)(p[0] & 0x1F);
			left = 1;
		}
	}
	else {
		if ((p[0] & 0x80) == 0x00) {
			ch = (Uint32)p[0];
		}
	}
	++*src;
	--*srclen;
	while (left > 0 && *srclen > 0) {
		++p;
		if ((p[0] & 0xC0) != 0x80) {
			ch = UNKNOWN_UNICODE;
			break;
		}
		ch <<= 6;
		ch |= (p[0] & 0x3F);
		++*src;
		--*srclen;
		--left;
	}
	if (left > 0) {
		underflow = SDL_TRUE;
	}
	/* Technically overlong sequences are invalid and should not be interpreted.
	However, it doesn't cause a security risk here and I don't see any harm in
	displaying them. The application is responsible for any other side effects
	of allowing overlong sequences (e.g. string compares failing, etc.)
	See bug 1931 for sample input that triggers this.
	*/
	/*if (overlong) return UNKNOWN_UNICODE;*/
	if (underflow ||
		(ch >= 0xD800 && ch <= 0xDFFF) ||
		(ch == 0xFFFE || ch == 0xFFFF) || ch > 0x10FFFF) {
		ch = UNKNOWN_UNICODE;
	}
	return ch;
}

void GameMath::splitUTF8String(const std::string& s, std::vector<Uint16>& chs)
{
	const char* text = s.c_str();
	size_t textlen = SDL_strlen(text);
	while (textlen > 0) 
	{
		Uint16 c = UTF8_getch(&text, &textlen);
		if (c == UNICODE_BOM_NATIVE || c == UNICODE_BOM_SWAPPED) 
		{
			continue;
		}
		chs.push_back(c);
	}
}

void GameMath::splitUTF8(const std::string& s, std::vector<std::string>& chs)
{
	for (int i = 0; s[i] != '\0';)
	{
		char chr = s[i];
		//chr是0xxx xxxx，即ascii码  
		if ((chr & 0x80) == 0)
		{
			chs.push_back(s.substr(i, 1));
			++i;
		}//chr是1111 1xxx  
		else if ((chr & 0xF8) == 0xF8)
		{
			chs.push_back(s.substr(i, 5));			
			i += 5;
		}//chr是1111 xxxx  
		else if ((chr & 0xF0) == 0xF0)
		{
			chs.push_back(s.substr(i, 4));
			i += 4;
		}//chr是111x xxxx  
		else if ((chr & 0xE0) == 0xE0)
		{
			chs.push_back(s.substr(i, 3));
			i += 3;
		}//chr是11xx xxxx  
		else if ((chr & 0xC0) == 0xC0)
		{
			chs.push_back(s.substr(i, 2));
			i += 2;
		}
	}
}

std::string GameMath::format(const char* format, ...)
{
	char buf[256] = { 0 };
	
	va_list args;
	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);

	return buf;
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

std::string IOManager::_buffer;

bool IOManager::readFileToBuffer(const std::string& filePath, std::vector<unsigned char> &buffer)
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

bool IOManager::readFileToBuffer(const std::string& filePath, std::string &buffer)
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

bool IOManager::readFileToBuffer(const std::string& filePath, std::stringstream &buf, bool ignore/* = false*/)
{
	SDL_RWops* fileOP = SDL_RWFromFile(filePath.c_str(), "r");
	if (fileOP == NULL)
	{
		if (!ignore)
		{
			perror(filePath.c_str());
		}
		
		return false;
	}

	int len = SDL_RWsize(fileOP);

	_buffer.resize(len);
	SDL_RWread(fileOP, &_buffer[0], len, 1);
	SDL_RWclose(fileOP);

	buf.clear();
	buf << _buffer;

	return true;
}

bool IOManager::writeFile(const std::string& filePath, std::string& buffer)
{
	SDL_RWops* fileOP = SDL_RWFromFile(filePath.c_str(), "w");
	SDL_RWwrite(fileOP, buffer.c_str(), buffer.size(), 1);
	SDL_RWclose(fileOP);

	return true;
}