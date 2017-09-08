#include "mobsprite.h"
#include "mob.h"
#include "texturefilm.h"
#include "dungeon.h"
#include "speck.h"
#include "alphatweener.h"
#include "gamescene.h"

const float MobSprite::FADE_TIME = 3.0f;
const float MobSprite::FALL_TIME = 1.0f;

void MobSprite::update()
{
	sleeping = ch != NULL && ((Mob*)ch)->state == ((Mob*)ch)->SLEEPEING;
	CharSprite::update();
}

namespace{
	class AlphaTweenerNew :public AlphaTweener{
	public:
		MobSprite* owner;
		AlphaTweenerNew(MobSprite* o, Visual* image, float alpha, float time)
			:owner(o),
			AlphaTweener(image, alpha, time)
		{

		}
	protected:
		virtual void onComplete()
		{
			owner->killAndErase();
			parent->erase(this);
		}
	};
}
void MobSprite::onComplete(Animation* anim)
{
	CharSprite::onComplete(anim);

	if (anim == die) 
	{
		parent->add(new AlphaTweenerNew(this, this, 0, FADE_TIME));
	}
}

RatSprite::RatSprite()
{
	texture(Assets::RAT);

	TextureFilm* frames = new TextureFilm(tex, 16, 15);

	idle = new Animation(2, true);
	int arry1[] = { 0, 0, 0, 1 };
	idle->Frames(frames, std::vector<int>(arry1, arry1+sizeof(arry1) / sizeof(int)));

	run = new Animation(10, true);
	int arry2[] = { 6, 7, 8, 9, 10 };
	run->Frames(frames, std::vector<int>(arry2, arry2 + sizeof(arry2) / sizeof(int)));

	attack = new Animation(15, false);
	int arry3[] = { 2, 3, 4, 5, 0 };
	attack->Frames(frames, std::vector<int>(arry3, arry3 + sizeof(arry3) / sizeof(int)));

	die = new Animation(10, false);
	int arry4[] = { 11, 12, 13, 14 };
	die->Frames(frames, std::vector<int>(arry4, arry4 + sizeof(arry4) / sizeof(int)));

	play(idle);

	delete frames;
}

GnollSprite::GnollSprite()
{
	texture(Assets::GNOLL);

	TextureFilm* frames = new TextureFilm(tex, 12, 15);

	idle = new Animation(2, true);
	int arry1[] = { 0, 0, 0, 1, 0, 0, 1, 1 };
	idle->Frames(frames, arry1, sizeof(arry1)/sizeof(int));

	run = new Animation(12, true);
	int arry2[] = { 4, 5, 6, 7 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 2, 3, 0 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(12, false);
	int arry4[] = { 8, 9, 10 };
	die->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);

	delete frames;
}

CrabSprite::CrabSprite()
{
	texture(Assets::CRAB);

	TextureFilm* frames = new TextureFilm(tex, 16);

	idle = new Animation(5, true);
	int arry1[] = { 0, 1, 0, 2 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, true);
	int arry2[] = { 3, 4, 5, 6 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 7, 8, 9 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(12, false);
	int arry4[] = { 10, 11, 12, 13 };
	die->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);

	delete frames;
}

SwarmSprite::SwarmSprite()
{
	texture(Assets::SWARM);

	TextureFilm* frames = new TextureFilm(tex, 16, 16);

	idle = new Animation(15, true);
	int arry1[] = { 0, 1, 2, 3, 4, 5 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, true);
	int arry2[] = { 0, 1, 2, 3, 4, 5 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(20, false);
	int arry3[] = { 6, 7, 8, 9 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(15, false);
	int arry4[] = { 10, 11, 12, 13, 14 };
	die->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);

	delete frames;
}

SkeletonSprite::SkeletonSprite()
{
	texture(Assets::SKELETON);

	TextureFilm* frames = new TextureFilm(tex, 12, 15);

	idle = new Animation(12, true);
	int arry1[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, true);
	int arry2[] = { 4, 5, 6, 7, 8, 9 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(15, false);
	int arry3[] = { 14, 15, 16 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(12, false);
	int arry4[] = { 10, 11, 12, 13 };
	die->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	play(idle);

	delete frames;
}

void SkeletonSprite::Die()
{
	MobSprite::Die();
	if (Dungeon::visible[ch->pos]) 
	{
		emitter()->burst(Speck::factory(Speck::BONE), 6);
	}
}

ThiefSprite::ThiefSprite()
{
	texture(Assets::THIEF);
	TextureFilm* film = new TextureFilm(tex, 12, 13);

	idle = new Animation(1, true);
	int arry1[] = { 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	idle->Frames(film, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, true);
	int arry2[] = { 0, 0, 2, 3, 3, 4 };
	run->Frames(film, arry2, sizeof(arry2) / sizeof(int));

	die = new Animation(10, false);
	int arry3[] = { 5, 6, 7, 8, 9 };
	die->Frames(film, arry3, sizeof(arry3) / sizeof(int));

	attack = new Animation(12, false);
	int arry4[] = { 10, 11, 12, 0 };
	attack->Frames(film, arry4, sizeof(arry4) / sizeof(int));

	Idle();

	delete film;
}

GooSprite::GooSprite()
{
	texture(Assets::GOO);

	TextureFilm* frames = new TextureFilm(tex, 20, 14);

	idle = new Animation(10, true);
	int arry1[] = { 0, 1 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(10, true);
	int arry2[] = { 0, 1 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	pump = new Animation(20, true);
	int arry3[] = { 0, 1 };
	pump->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	jump = new Animation(1, true);
	int arry4[] = { 6 };
	jump->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	attack = new Animation(10, false);
	int arry5[] = { 5, 0, 6 };
	attack->Frames(frames, arry5, sizeof(arry5) / sizeof(int));

	die = new Animation(10, false);
	int arry6[] = { 2, 3, 4 };
	die->Frames(frames, arry6, sizeof(arry6) / sizeof(int));

	MobSprite::play(idle);

	delete frames;
}

void GooSprite::play(Animation* anim, bool force)
{
	MobSprite::play(anim, force);

	if (anim == pump) 
	{
		spray = centerEmitter();
		spray->pour(GooParticle::FACTORY, 0.04f);
	}
	else if (spray != NULL) 
	{
		spray->on = false;
		spray = NULL;
	}
}

namespace{
	class FactoryNew :public Emitter::Factory{
	public:
		virtual void emit(Emitter* emitter, int index, float x, float y)
		{
			GooSprite::GooParticle* gp = (GooSprite::GooParticle*)emitter->recycle("GooParticle");
			if (gp == NULL)
			{
				gp = new GooSprite::GooParticle();
				emitter->add(gp);
			}
			gp->reset(x, y);
		}
	};
}
Emitter::Factory* GooSprite::GooParticle::FACTORY = new FactoryNew();

GooSprite::GooParticle::GooParticle()
{
	color(0x000000);
	_lifespan = 0.3f;

	GameMath::PointFSet(&acc, 0, +50);
	//acc.set(0, +50);
}

void GooSprite::GooParticle::reset(float x, float y)
{
	revive();

	this->x = x;
	this->y = y;

	_left = _lifespan;

	_sizef = 4;

	GameMath::PointFPolar(&speed, -Random::Float(GameMath::POINTF_PI), Random::Float(32, 48));
	//speed.polar(-Random.Float(PointF.PI), Random.Float(32, 48));
}

void GooSprite::GooParticle::update()
{
	Shrinking::update();
	float p = _left / _lifespan;
	am = p > 0.5f ? (1 - p) * 2.0f : 1;
}

ShamanSprite::ShamanSprite()
{
	texture(Assets::SHAMAN);

	TextureFilm* frames = new TextureFilm(tex, 12, 15);

	idle = new Animation(2, true);
	int arry1[] = { 0, 0, 0, 1, 0, 0, 1, 1 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(12, true);
	int arry2[] = { 4, 5, 6, 7 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 2, 3, 0 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	zap = attack->clone();

	die = new Animation(12, false);
	int arry4[] = { 8, 9, 10 };
	die->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);

	delete frames;
}

void ShamanSprite::Zap(int pos)
{
	points[0] = ch->pos;
	points[1] = pos;
	//parent.add(new Lightning(points, 2, (Shaman)ch));

	turnTo(ch->pos, pos);
	play(zap);
}

BatSprite::BatSprite()
{
	texture(Assets::BAT);

	TextureFilm* frames = new TextureFilm(tex, 15, 15);

	idle = new Animation(8, true);
	int arry1[] = { 0, 1 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(12, true);
	int arry2[] = { 0, 1 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 2, 3, 0, 1 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(12, false);
	int arry4[] = { 4, 5, 6 };
	die->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);

	delete frames;
}

BruteSprite::BruteSprite()
{
	texture(Assets::BRUTE);

	TextureFilm* frames = new TextureFilm(tex, 12, 16);

	idle = new Animation(2, true);
	int arry1[] = { 0, 0, 0, 1, 0, 0, 1, 1 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(12, true);
	int arry2[] = { 4, 5, 6, 7 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(12, false);
	int arry3[] = { 2, 3, 0 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	die = new Animation(12, false);
	int arry4[] = { 8, 9, 10 };
	die->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	play(idle);

	delete frames;
}

TenguSprite::TenguSprite()
{
	texture(Assets::TENGU);

	TextureFilm* frames = new TextureFilm(tex, 14, 16);

	idle = new Animation(2, true);
	int arry1[] = { 0, 0, 0, 1 };
	idle->Frames(frames, arry1, sizeof(arry1) / sizeof(int));

	run = new Animation(15, false);
	int arry2[] = { 2, 3, 4, 5, 0 };
	run->Frames(frames, arry2, sizeof(arry2) / sizeof(int));

	attack = new Animation(15, false);
	int arry3[] = { 6, 7, 7, 0 };
	attack->Frames(frames, arry3, sizeof(arry3) / sizeof(int));

	cast = attack->clone();

	die = new Animation(8, false);
	int arry4[] = { 8, 9, 10, 10, 10, 10, 10, 10 };
	die->Frames(frames, arry4, sizeof(arry4) / sizeof(int));

	play(run->clone());

	delete frames;
}

void TenguSprite::move(int from, int to)
{
	place(to);

	play(run);
	turnTo(from, to);

	isMoving = true;

	if (Level::water[to]) 
	{
		GameScene::ripple(to);
	}

	ch->onMotionComplete();
}

void TenguSprite::Attack(int cell)
{
	if (!Level::adjacent(cell, ch->pos)) 
	{
		//((MissileSprite)parent.recycle(MissileSprite.class)).
		//	reset(ch->pos, cell, new Shuriken(), new Callback(){
		//	@Override
		//	public void call() {
		//		ch.onAttackComplete();
		//	}
		//});

		play(cast);
		turnTo(ch->pos, cell);
	}
	else 
	{
		MobSprite::Attack(cell);
	}
}

void TenguSprite::onComplete(Animation* anim)
{
	if (anim == run) 
	{
		isMoving = false;
		Idle();
	}
	else 
	{
		MobSprite::onComplete(anim);
	}
}
