#include "game.h"
#include "menu.h"
#include <stdlib.h>
#include <time.h>

SDL_Window *sdlWindow = NULL;
SDL_Renderer *sdlRenderer = NULL;

int Init() {

	srand ( time(NULL) );

	//Initialize all SDL subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
		return false;
	}

	//Set up the screen
	if( !(sdlWindow = SDL_CreateWindow(
	                      "Snake v0.5",
	                      SDL_WINDOWPOS_UNDEFINED,
	                      SDL_WINDOWPOS_UNDEFINED,
	                      SCREEN_WIDTH,
	                      SCREEN_HEIGHT,
	                      SCREEN_FLAGS)) ) {
		return false;
	}

	if( !(sdlRenderer = SDL_CreateRenderer(sdlWindow, -1,
	                                       SDL_RENDERER_ACCELERATED)) ) {
		return false;
	}

	//Initialize SDL_ttf
	if( TTF_Init() == -1 ) {
		return false;
	}

	return true;
}

int main(int argc, char *args[]) {
	Game *game = NULL;
	Menu *menu = NULL;
	SDL_Surface *screen;

	Menu::selection sel;
	bool end;

	if(Init() == false) {
		return 0;
	}

	screen = SDL_GetWindowSurface( sdlWindow );

	menu = new Menu;

	if(menu->Init(screen->format) == false) {
		return 0;
	}

	end = false;

	while(end != true) {

		sel = menu->Main(sdlWindow);

		if(sel != Menu::QUIT) {

			game = new Game;

			if(game->Init(screen->format) == false) {
				return 0;
			}

			switch(sel) {

				case Menu::SINGLE:
					if(game->Execute(sdlWindow, Game::SINGLE) == false) {
						return 0;
					}

					break;

				case Menu::AI:
					if(game->Execute(sdlWindow, Game::AI) == false) {
						return 0;
					}

					break;

				case Menu::SMART_AI:
					if(game->Execute(sdlWindow, Game::SMART_AI) == false) {
						return 0;
					}

					break;

				default:
					break;
			}

			delete game;
			game = NULL;

		} else {
			end = true;
			break;
		}
	}

	if(game) {
		delete game;
	}

	if(menu) {
		delete menu;
	}

	if(sdlRenderer) {
		SDL_DestroyRenderer(sdlRenderer);
	}

	if(sdlWindow) {
		SDL_DestroyWindow(sdlWindow);
	}

	TTF_Quit();
	SDL_Quit();

	return 1;
}
