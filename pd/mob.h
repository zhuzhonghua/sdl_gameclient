#pragma once

#include "char.h"

class Mob :public Char{
public:
	class AiState 
	{
	public:
		AiState(Mob* m) :owner(m){}
		Mob* owner;
		virtual bool act(bool enemyInFOV, bool justAlerted) = 0;
		virtual std::string status() = 0;
	};

	Mob();

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
	virtual CharSprite* Sprite() = 0;

	virtual void add(Buff* buff);
	virtual void remove(Buff* buff);
	virtual void move(int step);
	virtual void onAttackComplete();
	virtual int defenseSkill(Char* enemy);
	virtual int defenseProc(Char* enemy, int damage);
	void aggro(Char* ch);
	virtual void notice();
	virtual void beckon(int cell);
	//virtual void damage(int dmg, Object src);
	void yell(const std::string& str);
	virtual std::string description() { return "Real description is coming soon!"; }

	bool hostile;

	AiState* SLEEPEING;
	AiState* HUNTING;
	AiState* WANDERING;
	AiState* FLEEING;
	AiState* PASSIVE;
	AiState* state;
protected:
	int target;

	int _defenseSkill;

	int EXP;
	int maxLvl;

	Char* enemy;
	bool enemySeen;
	bool alerted;

	static const float TIME_TO_WAKE_UP;

	float lootChance;
	
	virtual bool act();
	Char* chooseEnemy();
	bool moveSprite(int from, int to);
	virtual bool canAttack(Char* enemy);
	virtual bool getCloser(int target);
	virtual bool getFurther(int target);
	virtual float attackDelay() { return 1.0f; }
	virtual bool doAttack(Char* enemy);

	class Fleeing :public AiState{
	public:
		Fleeing(Mob* m) :AiState(m){}

		static const std::string TAG;
		virtual bool act(bool enemyInFOV, bool justAlerted);
		virtual std::string status();
	protected:
		void nowhereToRun() {}
	};
private:
	static const std::string STATE;
	static const std::string TARGET;

	class Sleeping :public AiState{
	public:
		Sleeping(Mob* m) :AiState(m){}
		static const std::string TAG;

		virtual bool act(bool enemyInFOV, bool justAlerted);
		virtual std::string status();
	};
	class Wandering :public AiState{
	public:
		static const std::string TAG;
		Wandering(Mob* m) :AiState(m){}

		virtual bool act(bool enemyInFOV, bool justAlerted);
		virtual std::string status();
	};
	class Hunting :public AiState{
	public:
		static const std::string TAG;

		Hunting(Mob* m) :AiState(m){}

		virtual bool act(bool enemyInFOV, bool justAlerted);
		virtual std::string status();
	};
	class Passive :public AiState{
	public:
		static const std::string TAG;
		Passive(Mob* m) :AiState(m){}

		virtual bool act(bool enemyInFOV, bool justAlerted);
		virtual std::string status();
	};
};

typedef Mob* (*CreateMob)();