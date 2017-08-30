#pragma once

#include "regularlevel.h"

class SewerLevel :public RegularLevel{
public:
	virtual std::string tilesTex();
	virtual std::string waterTex();

	virtual std::string getClassName() { return "SewerLevel"; }

protected:
	std::vector<bool> water();
	std::vector<bool> grass();
	virtual void decorate();
	virtual void createMobs();
	virtual void createItems();
};