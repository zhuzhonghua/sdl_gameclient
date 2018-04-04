
#include "test.h"
#include "SDL.h"
#include "SDL_image.h"

void testLoadIMG()
{
	const int SCREEN_WIDTH = 550;
	const int SCREEN_HEIGHT = 400;

	SDL_Window* gWindow = NULL;
	SDL_Renderer* renderer = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);
	gWindow = SDL_CreateWindow("MyGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(gWindow, -1, 0);

	SDL_Texture*tex = IMG_LoadTexture(renderer, "workground.jpg");
	SDL_Rect tex_rect;
	tex_rect.x = 10;
	tex_rect.y = 10;
	tex_rect.w = SCREEN_WIDTH;
	tex_rect.h = SCREEN_HEIGHT;
	SDL_RenderCopy(renderer, tex, NULL, &tex_rect);

	SDL_RenderPresent(renderer);

	SDL_Delay(4000);

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(gWindow);

	SDL_Quit();
}