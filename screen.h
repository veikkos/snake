#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "SDL2\SDL.h"
#include <string>

SDL_Surface *load_image( std::string filename, SDL_PixelFormat *pixelformat );
void apply_surface( int x, int y, SDL_Surface *source,
                    SDL_Surface *destination, SDL_Rect *clip = NULL );

#endif
