#ifndef __TAGLOADER_H__
#define __TAGLOADER_H__


class Movie;
struct  TagHeader;
class TagLoader
{
public:
	static void loadDefineBitsJPEG(Movie* m, const TagHeader* th);
	static void loadDefineShape(Movie* m, const TagHeader* th);
	static void loadPlaceObject(Movie* m, const TagHeader* th);
};
#endif