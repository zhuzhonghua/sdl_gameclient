#pragma once

#include <string>

class Char;
class LightningTrap{
private:
	static const std::string name;

public:
	static void trigger(int pos, Char* ch);
	class Electricity{};
	static Electricity* LIGHTNING;
};