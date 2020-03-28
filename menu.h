#ifndef _MENU_H_
#define _MENU_H_

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "screen.h"
#include "defines.h"

class Menu {
public:

	Menu();
	~Menu();

	bool Init(SDL_PixelFormat *pixelformat);

	typedef enum {
		NONE,
		QUIT,
		SINGLE,
		AI,
		SMART_AI
	} selection;

	selection Main(SDL_Window *window);

private:

	selection GetInput();
	int PrintText(SDL_Surface *screen, int x, int y, char *text,
	              TTF_Font *font, SDL_Color *color);

	SDL_Event event;
	SDL_Surface *background;

	TTF_Font *font;
};

#endif
