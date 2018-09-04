#pragma once

#include "blob.h"
//#include "hero.h"

class ToxicGas :public Blob, public Hero::Doom, public Object{
public:
	virtual std::string getClassName() { return "ToxicGas"; }
	CLASSOBJECT(ToxicGas);

	virtual void use(BlobEmitter* emitter);
	virtual std::string tileDesc();

	virtual void onDeath();
protected:
	virtual void evolve();
};