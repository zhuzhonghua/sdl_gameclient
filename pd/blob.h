#pragma once

#include "actor.h"
#include "level.h"

class BlobEmitter;
class Blob :public Actor{
public:
	static const int WIDTH = Level::WIDTH;
	static const int HEIGHT = Level::HEIGHT;
	static const int LENGTH = Level::LENGTH;

	int volume;

	std::vector<int> cur;

	BlobEmitter* emitter;

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
	virtual bool act();
	void use(BlobEmitter* emitter){ this->emitter = emitter; }
	void seed(int cell, int amount);
	void clear(int cell);
	std::string tileDesc() { return ""; }
	static Blob* seed(int cell, int amount, const std::string& type);
protected:
	std::vector<int> off;
	Blob();
	void evolve();
private:
	static const std::string CUR;
	static const std::string START;

	std::vector<int> trim(int start, int end);

	static Blob* getTypeInstance(const std::string& type);
};