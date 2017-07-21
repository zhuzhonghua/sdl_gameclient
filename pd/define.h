#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <string>

typedef glm::vec2 PointF;
typedef glm::ivec2 Point;
typedef glm::mat4 Matrix;
typedef glm::vec4 RectF;
typedef unsigned char byte;

namespace{
	const char* ARCS_BG = "data/pd/arcs1.png";
	const char* ARCS_FG = "data/pd/arcs2.png";
	const char* DASHBOARD = "data/pd/dashboard.png";

	const char* BANNERS = "data/pd/banners.png";
	const char* BADGES = "data/pd/badges.png";
	const char* LOCKED = "data/pd/locked_badge.png";
	const char* AMULET = "data/pd/amulet.png";

	const char* CHROME = "data/pd/chrome.png";
	const char* ICONS = "data/pd/icons.png";
	const char* STATUS = "data/pd/status_pane.png";
	const char* HP_BAR = "data/pd/hp_bar.png";
	const char* XP_BAR = "data/pd/exp_bar.png";
	const char* TOOLBAR = "data/pd/toolbar.png";
	const char* SHADOW = "data/pd/shadow.png";

	const char* WARRIOR = "data/pd/warrior.png";
	const char* MAGE = "data/pd/mage.png";
	const char* ROGUE = "data/pd/rogue.png";
	const char* HUNTRESS = "data/pd/ranger.png";
	const char* AVATARS = "data/pd/avatars.png";
	const char* PET = "data/pd/pet.png";

	const char* SURFACE = "data/pd/surface.png";

	const char* FIREBALL = "data/pd/fireball.png";
	const char* SPECKS = "data/pd/specks.png";
	const char* EFFECTS = "data/pd/effects.png";

	const char* RAT = "data/pd/rat.png";
	const char* GNOLL = "data/pd/gnoll.png";
	const char* CRAB = "data/pd/crab.png";
	const char* GOO = "data/pd/goo.png";
	const char* SWARM = "data/pd/swarm.png";
	const char* SKELETON = "data/pd/skeleton.png";
	const char* SHAMAN = "data/pd/shaman.png";
	const char* THIEF = "data/pd/thief.png";
	const char* TENGU = "data/pd/tengu.png";
	const char* SHEEP = "data/pd/sheep.png";
	const char* KEEPER = "data/pd/shopkeeper.png";
	const char* BAT = "data/pd/bat.png";
	const char* BRUTE = "data/pd/brute.png";
	const char* SPINNER = "data/pd/spinner.png";
	const char* DM300 = "data/pd/dm300.png";
	const char* WRAITH = "data/pd/wraith.png";
	const char* ELEMENTAL = "data/pd/elemental.png";
	const char* MONK = "data/pd/monk.png";
	const char* WARLOCK = "data/pd/warlock.png";
	const char* GOLEM = "data/pd/golem.png";
	const char* UNDEAD = "data/pd/undead.png";
	const char* KING = "data/pd/king.png";
	const char* STATUE = "data/pd/statue.png";
	const char* PIRANHA = "data/pd/piranha.png";
	const char* EYE = "data/pd/eye.png";
	const char* SUCCUBUS = "data/pd/succubus.png";
	const char* SCORPIO = "data/pd/scorpio.png";
	const char* ROTTING = "data/pd/rotting_fist.png";
	const char* BURNING = "data/pd/burning_fist.png";
	const char* YOG = "data/pd/yog.png";
	const char* LARVA = "data/pd/larva.png";
	const char* GHOST = "data/pd/ghost.png";
	const char* MAKER = "data/pd/wandmaker.png";
	const char* TROLL = "data/pd/blacksmith.png";
	const char* IMP = "data/pd/demon.png";
	const char* RATKING = "data/pd/ratking.png";
	const char* BEE = "data/pd/bee.png";
	const char* MIMIC = "data/pd/mimic.png";

	const char* ITEMS = "data/pd/items.png";
	const char* PLANTS = "data/pd/plants.png";

	const char* TILES_SEWERS = "data/pd/tiles0.png";
	const char* TILES_PRISON = "data/pd/tiles1.png";
	const char* TILES_CAVES = "data/pd/tiles2.png";
	const char* TILES_CITY = "data/pd/tiles3.png";
	const char* TILES_HALLS = "data/pd/tiles4.png";

	const char* WATER_SEWERS = "data/pd/water0.png";
	const char* WATER_PRISON = "data/pd/water1.png";
	const char* WATER_CAVES = "data/pd/water2.png";
	const char* WATER_CITY = "data/pd/water3.png";
	const char* WATER_HALLS = "data/pd/water4.png";

	const char* BUFFS_SMALL = "data/pd/buffs.png";
	const char* BUFFS_LARGE = "data/pd/large_buffs.png";
	const char* SPELL_ICONS = "data/pd/spell_icons.png";

	const char* FONTS1X = "data/pd/font1x.png";
	const char* FONTS15X = "data/pd/font15x.png";
	const char* FONTS2X = "data/pd/font2x.png";
	const char* FONTS25X = "data/pd/font25x.png";
	const char* FONTS3X = "data/pd/font3x.png";

	const char* THEME = "data/pd/theme.mp3";
	const char* TUNE = "data/pd/game.mp3";
	const char* HAPPY = "data/pd/surface.mp3";

	const char* SND_CLICK = "data/pd/snd_click.mp3";
	const char* SND_BADGE = "data/pd/snd_badge.mp3";
	const char* SND_GOLD = "data/pd/snd_gold.mp3";

	const char* SND_OPEN = "data/pd/snd_door_open.mp3";
	const char* SND_UNLOCK = "data/pd/snd_unlock.mp3";
	const char* SND_ITEM = "data/pd/snd_item.mp3";
	const char* SND_DEWDROP = "data/pd/snd_dewdrop.mp3";
	const char* SND_HIT = "data/pd/snd_hit.mp3";
	const char* SND_MISS = "data/pd/snd_miss.mp3";
	const char* SND_STEP = "data/pd/snd_step.mp3";
	const char* SND_WATER = "data/pd/snd_water.mp3";
	const char* SND_DESCEND = "data/pd/snd_descend.mp3";
	const char* SND_EAT = "data/pd/snd_eat.mp3";
	const char* SND_READ = "data/pd/snd_read.mp3";
	const char* SND_LULLABY = "data/pd/snd_lullaby.mp3";
	const char* SND_DRINK = "data/pd/snd_drink.mp3";
	const char* SND_SHATTER = "data/pd/snd_shatter.mp3";
	const char* SND_ZAP = "data/pd/snd_zap.mp3";
	const char* SND_LIGHTNING = "data/pd/snd_lightning.mp3";
	const char* SND_LEVELUP = "data/pd/snd_levelup.mp3";
	const char* SND_DEATH = "data/pd/snd_death.mp3";
	const char* SND_CHALLENGE = "data/pd/snd_challenge.mp3";
	const char* SND_CURSED = "data/pd/snd_cursed.mp3";
	const char* SND_TRAP = "data/pd/snd_trap.mp3";
	const char* SND_EVOKE = "data/pd/snd_evoke.mp3";
	const char* SND_TOMB = "data/pd/snd_tomb.mp3";
	const char* SND_ALERT = "data/pd/snd_alert.mp3";
	const char* SND_MELD = "data/pd/snd_meld.mp3";
	const char* SND_BOSS = "data/pd/snd_boss.mp3";
	const char* SND_BLAST = "data/pd/snd_blast.mp3";
	const char* SND_PLANT = "data/pd/snd_plant.mp3";
	const char* SND_RAY = "data/pd/snd_ray.mp3";
	const char* SND_BEACON = "data/pd/snd_beacon.mp3";
	const char* SND_TELEPORT = "data/pd/snd_teleport.mp3";
	const char* SND_CHARMS = "data/pd/snd_charms.mp3";
	const char* SND_MASTERY = "data/pd/snd_mastery.mp3";
	const char* SND_PUFF = "data/pd/snd_puff.mp3";
	const char* SND_ROCKS = "data/pd/snd_rocks.mp3";
	const char* SND_BURNING = "data/pd/snd_burning.mp3";
	const char* SND_FALLING = "data/pd/snd_falling.mp3";
	const char* SND_GHOST = "data/pd/snd_ghost.mp3";
	const char* SND_SECRET = "data/pd/snd_secret.mp3";
	const char* SND_BONES = "data/pd/snd_bones.mp3";
	const char* SND_BEE = "data/pd/snd_bee.mp3";
	const char* SND_DEGRADE = "data/pd/snd_degrade.mp3";
	const char* SND_MIMIC = "data/pd/snd_mimic.mp3";
}