#pragma once

#include <time.h>
#include <random>
#include "define.h"

class Random{
	static Random* _inst;
	static Random* inst();

	Random();
	std::mt19937 randomEngine;
public:
	static float Float(float begin, float end);
	static float Float(float end);
	static int Int(int begin, int end);
	static int Int(int end);
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
	static PointF* PointFSet(PointF* p, float v);
	static PointF* PointFSet(PointF* p, float x, float y);
	static PointF* PointFPolar(PointF* p, float a, float l);

	static void splitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
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
};