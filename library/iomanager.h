#pragma once

#include "stdafx.h"

namespace Zhu{
	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string filePath, std::vector<char> &buffer);
	};
}