#include "quad.h"

std::vector<short> Quad::_indices;

std::vector<short>* Quad::getIndices(int size)
{
	if (size > _indices.size())
	{
		_indices.resize(size*SIZE);

		int pos = 0;
		int limit = size * 4;
		for (int ofs = 0; ofs < limit; ofs += 4)
		{
			_indices[pos++] = ofs + 0;
			_indices[pos++] = ofs + 1;
			_indices[pos++] = ofs + 2;
			_indices[pos++] = ofs + 0;
			_indices[pos++] = ofs + 2;
			_indices[pos++] = ofs + 3;
		}
	}

	return &_indices;
}