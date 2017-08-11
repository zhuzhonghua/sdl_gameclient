#include "window.h"
#include "simpleresource.h"
#include "pixelscene.h"
#include "game.h"

namespace{
	class NewTouchArea :public TouchArea{
	public:
		Window* win;

		NewTouchArea(Window* w, float x, float y, float width, float height) :TouchArea(x, y, width, height)
		{
			win = w;
		}
	protected:
		virtual void onClick(TouchScreen::Touch* touch)
		{
			if (!win->chrome->overlapsScreenPoint(
				(int)touch->current.x,
				(int)touch->current.y)) 
			{

				win->onBackPressed();
			}
		}
	};	
}
Window::Window()
:Window(0, 0, Chrome::get(Chrome::Type::WINDOW))
{
}

Window::Window(int width, int height)
: Window(width, height, Chrome::get(Chrome::Type::WINDOW))
{

}

Window::Window(int width, int height, NinePatch* chrome)
: Group()
{
	blocker = new NewTouchArea(this, 0, 0, PixelScene::uiCamera->width, PixelScene::uiCamera->height);
	blocker->cameraf = PixelScene::uiCamera;
	add(blocker);

	this->chrome = chrome;

	this->width = width;
	this->height = height;

	shadow = new ShadowBox();
	shadow->am = 0.5f;
	shadow->cameraf = PixelScene::uiCamera->visible ? PixelScene::uiCamera : Camera::mainCamera;
	add(shadow);

	chrome->x = -chrome->marginLeft();
	chrome->y = -chrome->marginTop();
	chrome->size(
		width - chrome->x + chrome->marginRight(),
		height - chrome->y + chrome->marginBottom());
	add(chrome);

	cameraf = new Camera(0, 0,
		(int)chrome->width,
		(int)chrome->height,
		PixelScene::defaultZoom);
	cameraf->x = (int)(Game::width - cameraf->width * cameraf->zoomf) / 2;
	cameraf->y = (int)(Game::height - cameraf->height * cameraf->zoomf) / 2;
	GameMath::PointFSet(&cameraf->scroll, chrome->x, chrome->y);
	//cameraf->scroll.set(chrome->x, chrome->y);
	Camera::add(cameraf);

	shadow->boxRect(
		cameraf->x / cameraf->zoomf,
		cameraf->y / cameraf->zoomf,
		chrome->Width(), chrome->height);

	Keys::event.add(this);
}

void Window::resize(int w, int h)
{
	this->width = w;
	this->height = h;

	chrome->size(
		width + chrome->marginHor(),
		height + chrome->marginVer());

	cameraf->resize((int)chrome->width, (int)chrome->height);
	cameraf->x = (int)(Game::width - cameraf->screenWidth()) / 2;
	cameraf->y = (int)(Game::height - cameraf->screenHeight()) / 2;

	shadow->boxRect(cameraf->x / cameraf->zoomf, cameraf->y / cameraf->zoomf, chrome->Width(), chrome->height);
}

void Window::destroy()
{
	Group::destroy();

	Camera::remove(cameraf);
	Keys::event.remove(this);
}

void Window::onSignal(Keys::Key* key)
{
	/*if (key.pressed) {
		switch (key.code) {
		case Keys.BACK:
			onBackPressed();
			break;
		case Keys.MENU:
			onMenuPressed();
			break;
		}
	}*/

	Keys::event.cancel();
}