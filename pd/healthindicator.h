#pragma once

#include "component.h"

class Char;
class Image;

class HealthIndicator :public Component{
private:
	static const float HEIGHT;

	Char* target;

	Image* bg;
	Image* level;
public:
	static HealthIndicator* instance;

	HealthIndicator();

	virtual void update();
	void Target(Char* ch);
	Char* Target() { return target; }
protected:
	virtual void createChildren();
};