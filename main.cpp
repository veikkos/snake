#include "game.h"
#include "menu.h"
#include <stdlib.h>
#include <time.h>

SDL_Surface *screen;

int Init(){

    srand ( time(NULL) );

    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SCREEN_FLAGS );

    //Set the window caption
    SDL_WM_SetCaption( "Snake v0.4", NULL );

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    return true;
}

int main(int argc, char* args[])
{
    Game *game = NULL;
    Menu *menu = NULL;

    Menu::selection sel;
    bool end;

    if(Init() == false)
        return 0;

    menu = new Menu;

    if(menu->Init() == false)
        return 0;

    end = false;

    while(end != true){

        sel = menu->Main(screen);

        if(sel != Menu::QUIT){

            game = new Game;

            if(game->Init() == false)
                return 0;

            switch(sel){

                case Menu::SINGLE:
                    if(game->Execute(screen, Game::SINGLE) == false)
                        return 0;
                    break;
                case Menu::AI:
                    if(game->Execute(screen, Game::AI) == false)
                        return 0;
                    break;
                case Menu::SMART_AI:
                    if(game->Execute(screen, Game::SMART_AI) == false)
                        return 0;
                    break;
                default:
                    break;
            }

            delete game;
            game = NULL;

        }else{
            end = true;
            break;
        }
    }

    if(game)
        delete game;

    if(menu)
        delete menu;

    TTF_Quit();
	SDL_Quit();

    return 1;
}
