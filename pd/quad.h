#pragma once

#include <vector>

class Quad{
public:
	static const int SIZE = 6;

	static std::vector<short>* getIndices(int size);
private:
	static std::vector<short> _indices;
};