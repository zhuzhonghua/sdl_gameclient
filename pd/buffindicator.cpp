#include "buffindicator.h"
#include "dungeon.h"
#include "texturecache.h"
#include "define.h"
#include "texturefilm.h"

BuffIndicator* BuffIndicator::heroInstance;

BuffIndicator::BuffIndicator(Char* ch)
{
	init();
	this->ch = ch;

	if (ch == Dungeon::hero)
	{
		heroInstance = this;
	}
}

void BuffIndicator::destroy()
{
	Component::destroy();
	if (this == heroInstance)
	{
		heroInstance = NULL;
	}
}

void BuffIndicator::refreshHero()
{
	if (heroInstance != NULL)
	{
		heroInstance->layout();
	}
}

void BuffIndicator::createChildren()
{
	texture = TextureCache::getTexture(Assets::BUFFS_SMALL);
	film = new TextureFilm(texture, SIZE, SIZE);
}

void BuffIndicator::layout()
{
	clear();

	//SparseArray<Image> newIcons = new SparseArray<Image>();
	//
	//for (Buff buff : ch.buffs()) {
	//	int icon = buff.icon();
	//	if (icon != NONE) {
	//		Image img = new Image(texture);
	//		img.frame(film.get(icon));
	//		img.x = x + members.size() * (SIZE + 2);
	//		img.y = y;
	//		add(img);
	//
	//		newIcons.put(icon, img);
	//	}
	//}
	//
	//for (Integer key : icons.keyArray()) {
	//	if (newIcons.get(key) == null) {
	//		Image icon = icons.get(key);
	//		icon.origin.set(SIZE / 2);
	//		add(icon);
	//		add(new AlphaTweener(icon, 0, 0.6f){
	//			@Override
	//			protected void updateValues(float progress) {
	//				super.updateValues(progress);
	//				image.scale.set(1 + 5 * progress);
	//			};
	//		});
	//	}
	//}
	//
	//icons = newIcons;
}
