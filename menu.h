#ifndef _MENU_H_
#define _MENU_H_

#include "SDL\SDL.h"
#include "SDL\SDL_ttf.h"
#include "SDL\SDL_image.h"
#include "screen.h"
#include "defines.h"

class Menu
{
    public:

        Menu();
        ~Menu();

        bool Init();
        void Main(SDL_Surface *screen, int *selection);

    private:

        int GetInput();
        int PrintText(SDL_Surface *screen, int x, int y, char* text, TTF_Font *font, SDL_Color *color);

        SDL_Event event;
        SDL_Surface *background;

        TTF_Font *font;
};

#endif
