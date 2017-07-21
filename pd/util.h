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
	static int Int(int begin, int end);
};

class GameMath {
public:
	static float speed(float speed, float acc);
	static float gate(float min, float value, float max);
	static void MATRIXRotate(Matrix& mat, float angle);
	static void MATRIXScale(Matrix& mat, float sx, float sy);
	static float RECTFWidth(const RectF& frame);
	static float RECTFHeight(const RectF& frame);
	static RectF RECTF(float left, float top, float right, float bottom);
	static float RECTFLeft(const RectF& frame);
	static float RECTFTop(const RectF& frame);
	static float RECTFRight(const RectF& frame);
	static float RECTFBottom(const RectF& frame);
	static PointF* PointFSet(PointF* p, float v);
	static PointF* PointFSet(PointF* p, float x, float y);
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
public:
	static bool readFileToBuffer(std::string filePath, std::vector<unsigned char> &buffer);
	static bool readFileToBuffer(std::string filePath, std::string &buffer);
};