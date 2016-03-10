#ifndef __CHARACTER_H__
#define __CHARACTER_H__

struct SDL_Texture;
class Character
{
public:
	Character(SDL_Texture* tex, int type);
	~Character();
private:
	int				_type;

	SDL_Texture*	_tex;
};
#endif