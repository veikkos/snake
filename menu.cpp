#include "menu.h"
#include "screen.h"

Menu::Menu(){

    background = NULL;
    font = NULL;
}

Menu::~Menu(){

    if(background != NULL)
        SDL_FreeSurface(background);

    if(font != NULL)
        TTF_CloseFont( font );
}

bool Menu::Init(){

    //Open the font
    font = TTF_OpenFont( "fonts/DigitalDream.ttf", 28 );

     //If there was an error in loading the font
    if( font == NULL )
         return false;

    background = load_image( "img/m_bg.png" );

    if( background == NULL )
        return false;

    return true;
}


void Menu::Main(SDL_Surface *screen, int *selection){

    int i_selection = 0;
    int x_pos = (SCREEN_WIDTH - 440) / 2;

    SDL_Color textColor = { 230, 230, 230 };

    //Draw background to the screen
    apply_surface( 0, 0, background, screen, NULL );

    PrintText(screen, x_pos, 60, (char*)"Select 1 to play", font, &textColor);
    PrintText(screen, x_pos, 100, (char*)"Select 2 to AI play", font, &textColor);
    PrintText(screen, x_pos, 140, (char*)"Select 3 to AI2 play", font, &textColor);
    PrintText(screen, x_pos, 180, (char*)"Select ESC to quit", font, &textColor);

    //Update the screen
    SDL_Flip( screen );

    while(i_selection == 0){
        SDL_Delay(10);
        i_selection = GetInput();
    }

    *selection = i_selection;
}

int Menu::PrintText(SDL_Surface *screen, int x, int y, char* text, TTF_Font *s_font, SDL_Color *color){

    SDL_Surface *m_text = NULL;

    m_text = TTF_RenderText_Solid( s_font, text, *color );
    apply_surface( x, y, m_text, screen );

    SDL_FreeSurface(m_text);

    return true;
}

int Menu::GetInput()
{
	while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT){
            return -1;

        //If a key was pressed
        }else if( event.type == SDL_KEYDOWN ) {

            //Set the proper message surface
            switch( event.key.keysym.sym )
            {
                case SDLK_1: return 1;
                case SDLK_2: return 2;
                case SDLK_3: return 3;
                case SDLK_ESCAPE: return -1;
                default: break;
            }
        }
    }

    return 0;
}
