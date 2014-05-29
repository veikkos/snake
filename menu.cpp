#include "menu.h"
#include "screen.h"

Menu::Menu() {

	background = NULL;
	font = NULL;
}

Menu::~Menu() {

	if(background != NULL) {
		SDL_FreeSurface( background );
	}

	if(font != NULL) {
		TTF_CloseFont( font );
	}
}

bool Menu::Init() {

	//Open the font
	font = TTF_OpenFont( "fonts/DigitalDream.ttf", 28 );

	//If there was an error in loading the font
	if( font == NULL ) {
		return false;
	}

	background = load_image( "img/m_bg.png" );

	if( background == NULL ) {
		return false;
	}

	return true;
}


Menu::selection Menu::Main(SDL_Window *window) {

	SDL_Surface *screen = SDL_GetWindowSurface( window );

	Menu::selection i_selection = Menu::NONE;
	int x_pos = (SCREEN_WIDTH - 440) / 2;

	SDL_Color textColor = { 230, 230, 230 };

	//Draw background to the screen
	apply_surface( 0, 0, background, screen, NULL );

	PrintText(screen, x_pos, 60, (char *)"Select 1 to play", font, &textColor);
	PrintText(screen, x_pos, 100, (char *)"Select 2 to AI play", font,
	          &textColor);
	PrintText(screen, x_pos, 140, (char *)"Select 3 to AI2 play", font,
	          &textColor);
	PrintText(screen, x_pos, 180, (char *)"Select ESC to quit", font,
	          &textColor);

	SDL_UpdateWindowSurface( window );

	while(i_selection == Menu::NONE) {
		SDL_Delay(10);
		i_selection = GetInput();
	}

	return i_selection;
}

int Menu::PrintText(SDL_Surface *screen, int x, int y, char *text,
                    TTF_Font *s_font, SDL_Color *color) {

	SDL_Surface *m_text = NULL;

	m_text = TTF_RenderText_Solid( s_font, text, *color );
	apply_surface( x, y, m_text, screen );

	SDL_FreeSurface( m_text );

	return true;
}

Menu::selection Menu::GetInput() {
	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT) {
			return Menu::QUIT;

			//If a key was pressed
		} else if( event.type == SDL_KEYDOWN ) {

			//Set the proper message surface
			switch( event.key.keysym.sym ) {
				case SDLK_1:
					return Menu::SINGLE;

				case SDLK_2:
					return Menu::AI;

				case SDLK_3:
					return Menu::SMART_AI;

				case SDLK_ESCAPE:
					return Menu::QUIT;

				default:
					break;
			}
		}
	}

	return Menu::NONE;
}
