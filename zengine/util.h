#pragma once

#include <string>
#include <vector>

namespace Zengine{
namespace Util{
void splitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
void splitStringRe(const std::string& s, std::vector<std::string>& v, pcrecpp::RE re);
void splitUTF8String(const std::string& s, std::vector<Uint16>& chs);
void splitUTF8(const std::string& s, std::vector<std::string>& chs);

std::string format(const char* format, ...);
}
}