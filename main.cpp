#include "game.h"
#include "menu.h"
#include <stdlib.h>
#include <time.h>

#define QUIT        -1
#define SINGLE      1
#define AI          2
#define SMART_AI    3

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
    SDL_WM_SetCaption( "Snake v0.3", NULL );

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    return true;
}

int main(int argc, char* args[])
{
    Game *game;
    Menu *menu;

    int selection;
    bool end;

    if(Init() == false)
        return 0;

    game = new Game;
    menu = new Menu;

    if(game->Init() == false)
        return 0;

    if(menu->Init() == false)
        return 0;

    end = false;

    while(end != true){

        selection = 0;
        menu->Main(screen, &selection);

        switch(selection){

            case SINGLE:
                if(game->Execute(screen) == false)
                    return 0;

                delete game;
                game = new Game;

                if(game->Init() == false)
                    return 0;

                break;

            case AI:
                if(game->Execute(screen, 1) == false)
                    return 0;

                delete game;
                game = new Game;

                if(game->Init() == false)
                    return 0;

                break;

            case SMART_AI:
                if(game->Execute(screen, 1, 1) == false)
                    return 0;

                delete game;
                game = new Game;

                if(game->Init() == false)
                    return 0;

                break;
            case QUIT:
                end = true;
                break;
            default:
                break;
        }
    }

    delete game;
    delete menu;

    TTF_Quit();
	SDL_Quit();

    return 1;
}
