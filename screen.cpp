#include "SDL2\SDL.h"
#include "SDL2\SDL_image.h"
#include <string>

SDL_Surface *load_image( std::string filename, SDL_PixelFormat *pixelformat ) {

	SDL_Surface *optimizedImage = NULL;
	//The image that's loaded
	SDL_Surface *loadedImage = NULL;

	//Load the image
	loadedImage = IMG_Load( filename.c_str() );

	if(loadedImage){

		optimizedImage = SDL_ConvertSurface(loadedImage, pixelformat, 0);

		SDL_FreeSurface(loadedImage);
	}

	//Return the optimized image
	return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface *source,
                    SDL_Surface *destination, SDL_Rect *clip = NULL ) {
	//Holds offsets
	SDL_Rect offset;

	//Get offsets
	offset.x = x;
	offset.y = y;

	//Blit
	SDL_BlitSurface( source, clip, destination, &offset );
}
