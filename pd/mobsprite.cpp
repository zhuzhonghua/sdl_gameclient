#include "mobsprite.h"
#include "mob.h"
#include "texturefilm.h"
#include "dungeon.h"
#include "speck.h"
#include "alphatweener.h"

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
