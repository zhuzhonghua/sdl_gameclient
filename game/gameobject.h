#ifndef __OBJECT_H__
#define __OBJECT_H__

class GameObject
{
protected:
	SDL_Rect		rect;
	int				z;
public:
	GameObject():z(0){}
	
	virtual ~GameObject(){}

	virtual void update(){}
	virtual void draw(){}
	virtual void handleEvent(SDL_Event* e){}

	virtual void setZ(int pz){ z = pz; }

	int		getZ() { return z; }
	virtual void setPosition(int px, int py)
	{
		rect.x = px;
		rect.y = py;
	}
	void getPosition(int* px, int *py)
	{
		*px = rect.x;
		*py = rect.y;
	}
	void setBound(int w, int h)
	{
		rect.w = w;
		rect.h = h;
	}
	void getBound(int* w, int* h)
	{
		*w = rect.w;
		*h = rect.h;
	}
};
#endif