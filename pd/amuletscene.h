#include "pixelscene.h"

class AmuletScene :public PixelScene{
private:
	static final String TXT_EXIT;
	static final String TXT_STAY;

	static final int WIDTH = 120;
	static final int BTN_HEIGHT = 18;
	static final float SMALL_GAP;
	static final float LARGE_GAP;

	static final String TXT;

	Image* amulet;

	float timer;
public:
	AmuletScene();
	static boolean noText;

	virtual void init();

	virtual void onBackPressed();

	virtual void update();
};