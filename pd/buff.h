#pragma once

#include "actor.h"
#include "typedefine.h"
#include "bpt.h"

class FlavourBuff;

class Buff :public Actor{
public:
	Buff();
	Char* target;

	virtual bool attachTo(Char* target);
	void detach();

	virtual bool act() {
		diactivate();
		return true;
	}

	virtual int icon();

	static Buff* append(Char* target, const std::string& buff);
	static FlavourBuff* append(Char* target, const std::string& buffClass, float duration);
	static Buff* affect(Char* target, const std::string& buffClass);
	static FlavourBuff* affect(Char* target, const std::string& buffClass, float duration);
	static FlavourBuff* prolong(Char* target, const std::string& buffClass, float duration);

	static void detach(Buff* buff);
	static void detach(Char* target, const std::string& cl);
};

FACTORY(Buff);

class FlavourBuff :public Buff{
public:
	virtual bool act() { detach(); return true; }
	virtual std::string getClassName(){ return "FlavourBuff"; }
};

class Light :public FlavourBuff{
public:
	CLASSNAME(Light);

	static const float DURATION;
	static const int DISTANCE = 4;

	virtual boolean attachTo(Char* target);
	virtual void detach();

	virtual int icon();

	virtual String toString() {
		return BPT::getText("lang.light");
	}
};

class BuffParalysis :public FlavourBuff{
private:
	static const float DURATION;
public:
	CLASSNAME(BuffParalysis);

	virtual boolean attachTo(Char* target);
	virtual void detach();
	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.buffparalysis");
	}
	static float duration(Char* ch);
	static void unfreeze(Char* ch);
};

class Frost :public FlavourBuff{
private:
	static const float DURATION;

public:
	CLASSNAME(Frost);

	virtual boolean attachTo(Char* target);
	virtual void detach();

	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.frost");
	}
	static float duration(Char* ch);
};

class BuffSlow :public FlavourBuff{
private:
	static const float DURATION;

public:
	CLASSNAME(BuffSlow);

	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.buffslow_str");
	}
	static float duration(Char* ch);
};

class Vertigo :public FlavourBuff{
public:
	static const float DURATION;

	CLASSNAME(Vertigo);

	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.vertigo_str");
	}
	static float duration(Char* ch);
};

class Terror :public FlavourBuff{
private:
	static const String OBJECT;
public:
	static const float DURATION;

	int object;
	
	Terror();
	CLASSNAME(Terror);

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.terror_str");
	}
	static void recover(Char* target);
};

class Cripple :public FlavourBuff{
public:
	CLASSNAME(Cripple);

	static const float DURATION;

	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.crippled_str");;
	}
};

class Bleeding :public Buff{
protected:
	int level;

private:
	static const String LEVEL;

public:
	CLASSNAME(Bleeding);

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	void set(int level) {
		this->level = level;
	};
	virtual int icon();
	virtual String toString() {
		return BPT::getText("lang.Bleeding");
	}
	virtual boolean act();
};