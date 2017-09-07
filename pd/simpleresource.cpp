#include "simpleresource.h"
#include "define.h"
#include "image.h"

NinePatch* Chrome::get(Type type)
{
	switch (type) 
	{
	case WINDOW:
		return new NinePatch(Assets::CHROME, 0, 0, 22, 22, 7);
	case TOAST:
		return new NinePatch(Assets::CHROME, 22, 0, 18, 18, 5);
	case TOAST_TR:
		return new NinePatch(Assets::CHROME, 40, 0, 18, 18, 5);
	case BUTTON:
		return new NinePatch(Assets::CHROME, 58, 0, 6, 6, 2);
	case TAG:
		return new NinePatch(Assets::CHROME, 22, 18, 16, 14, 3);
	case SCROLL:
		return new NinePatch(Assets::CHROME, 32, 32, 32, 32, 5, 11, 5, 11);
	case TAB_SET:
		return new NinePatch(Assets::CHROME, 64, 0, 22, 22, 7, 7, 7, 7);
	case TAB_SELECTED:
		return new NinePatch(Assets::CHROME, 64, 22, 10, 14, 4, 7, 4, 6);
	case TAB_UNSELECTED:
		return new NinePatch(Assets::CHROME, 74, 22, 10, 14, 4, 7, 4, 6);
	default:
		return NULL;
	}
}

Image* Effects::get(Type type)
{
	Image* icon = new Image(Assets::EFFECTS);
	switch (type) 
	{
	case RIPPLE:
		icon->frame(icon->tex->uvRect(0, 0, 16, 16));
		break;
	case LIGHTNING:
		icon->frame(icon->tex->uvRect(16, 0, 32, 8));
		break;
	case WOUND:
		icon->frame(icon->tex->uvRect(16, 8, 32, 16));
		break;
	case RAY:
		icon->frame(icon->tex->uvRect(16, 16, 32, 24));
		break;
	}
	return icon;
}

#include "terrain.h"
int Terrain::flags[256] = {0};
Terrain Terrain::inst;

const std::string ResultDescriptions::MOB = "Killed by %s on level %d";
const std::string ResultDescriptions::BOSS = "Killed by the %s on level %d";

// Items
const std::string ResultDescriptions::WAND = "Killed by your own %s on level %d";
const std::string ResultDescriptions::GLYPH = "Killed by the %s on level %d";
const std::string ResultDescriptions::BOMB = "Killed by an explosion of a bomb on level %d";

// Dungeon features
const std::string ResultDescriptions::TRAP = "Killed by discharge of %s on level %d";

// Debuffs & blobs
const std::string ResultDescriptions::BURNING = "Burned to death on level %d";
const std::string ResultDescriptions::HUNGER = "Starved to death on level %d";
const std::string ResultDescriptions::POISON = "Died from poison on level %d";
const std::string ResultDescriptions::GAS = "Died from toxic gas on level %d";
const std::string ResultDescriptions::BLEEDING = "Bled to death on level %d";
const std::string ResultDescriptions::OOZE = "Killed by a caustic ooze on level %d";
const std::string ResultDescriptions::FALL = "Fell to death on level %d";

const std::string ResultDescriptions::WIN = "Obtained the Amulet of Yendor";