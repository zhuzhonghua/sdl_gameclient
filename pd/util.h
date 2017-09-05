#pragma once

#include <time.h>
#include <random>
#include <set>
#include "define.h"
#include "regex/pcrecpp.h"

class Room;
class Random{
	static Random* _inst;
	static Random* inst();

	Random();
	std::mt19937 randomEngine;
public:
	static float Float(float begin, float end);
	static float Float(float end);
	static float Float();
	static float random();
	static int Int(int begin, int end);
	static int IntRange(int begin, int end);
	static int Int(int end);
	static Room* element(const std::set<Room*>& s);
};

class GameMath {
public:
	static const float POINTF_PI;
	static const float POINTF_PI2;
	static const float POINTF_G2R;

	static float speed(float speed, float acc);
	static float gate(float min, float value, float max);
	static void MATRIXRotate(Matrix& mat, float angle);
	static void MATRIXScale(Matrix& mat, float sx, float sy);
	static void RECTFOffset(RectF& frame, float dx, float dy);
	static float RECTFWidth(const RectF& frame);
	static float RECTFHeight(const RectF& frame);
	static RectF RECTF(float left, float top, float right, float bottom);
	static float RECTFLeft(const RectF& frame);
	static float RECTFTop(const RectF& frame);
	static float RECTFRight(const RectF& frame);
	static float RECTFBottom(const RectF& frame);
	
	static Rect RECT(int left, int top, int right, int bottom);
	static Rect RECTEmpty();
	static Rect RECTIntersect(const Rect& a, const Rect& b);
	static int RECTWidth(const Rect& frame);
	static int RECTHeight(const Rect& frame);
	static int RECTLeft(const Rect& frame);
	static int RECTTop(const Rect& frame);
	static int RECTRight(const Rect& frame);
	static int RECTBottom(const Rect& frame);
	static int RECTSquare(const Rect& frame);
	static bool isRECTEmpty(const Rect& frame);

	static PointF* PointFSet(PointF* p, float v);
	static PointF* PointFSet(PointF* p, float x, float y);
	static PointF* PointFOffSet(PointF* p, PointF dp);
	static PointF* PointFOffSet(PointF* p, PointF* dp);
	static PointF* PointFOffSet(PointF* p, float dx, float dy);
	static PointF* PointFPolar(PointF* p, float a, float l);
	static PointF* PointFScale(PointF* p, float f);
	static PointF* PointFInvScale(PointF* p, float f);
	static PointF* PointFNegate(PointF* p);
	static Point PointFFloor(PointF* a);
	static PointF PointFInter(PointF a, PointF b, float d);
	static float PointFAngle(PointF start, PointF end);
	static float PointFDistance(PointF a, PointF b);
	static PointF PointFDiff(PointF a, PointF b);
	static bool PointFEqual(PointF a, PointF b);

	static void splitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
	static void splitStringRe(const std::string& s, std::vector<std::string>& v, pcrecpp::RE re);
	static void splitUTF8String(const std::string& s, std::vector<Uint16>& chs);
	static void splitUTF8(const std::string& s, std::vector<std::string>& chs);

	static std::string format(const char* format, ...);
};

class ColorMath 
{
public: 
	static int interpolate(int A, int B, float p);
	static int interpolate(float p, const std::vector<int>& colors);
	static int random(int a, int b);
};

class IOManager
{
protected:
	static std::string _buffer;
public:	
	static bool readFileToBuffer(const std::string& filePath, std::vector<unsigned char> &buffer);
	static bool readFileToBuffer(const std::string& filePath, std::string &buffer);
	static bool readFileToBuffer(const std::string& filePath, std::stringstream &buffer, bool ignore=false);
	static bool writeFile(const std::string& filePath, std::string& buffer);
};

class FloatBuffer{
protected:
	int pos;
	std::vector<float> buf;
public:
	FloatBuffer(int size);
	void position(int pos);
	void put(const std::vector<float>& indices);
	float* buffer() { return &buf[0]; }
};

class Patch{
private:
	static std::vector<bool> cur;
	static std::vector<bool> off;

public:
	static std::vector<bool> generate(float seed, int nGen);
};

template<class T>
class Arrays{
public:
	static void fill(std::vector<T>& arr, T d)
	{
		for (int i = 0; i < arr.size(); i++)
		{
			arr[i] = d;
		}
	}
	static void fill(std::vector<T>& arr, int from, int to, T d)
	{
		for (int i = from; i < to; i++)
		{
			arr[i] = d;
		}
	}
	static void arraycopy(std::vector<T>& src, int srcFrom, std::vector<T>& dst, int dstFrom, int len)
	{
		for (int i = 0; i < len; i++)
		{
			dst[dstFrom + i] = src[srcFrom + i];
		}
	}
};

class BArray{
public:
	static std::vector<bool> not(std::vector<bool>&a, std::vector<bool>& result);
	static std::vector<bool> or(std::vector<bool>& a, std::vector<bool>& b, std::vector<bool>& result);
};