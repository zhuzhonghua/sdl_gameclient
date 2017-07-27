#pragma once

#include <vector>

class Quad{
public:
	static const int SIZE = 6;

	static std::vector<short>* getIndices(int size);
	static void fill(float v[],
		float x1, float x2, float y1, float y2,
		float u1, float u2, float v1, float v2);
private:
	static std::vector<short> _indices;
};