#include "game.h"
#include "defines.h"

Game::Game()
{
    background = NULL;
    snake_t = NULL;
    eatable = NULL;
    dyn_eatable = NULL;
    score_text = NULL;
    end_text = NULL;

    font = NULL;

    snake = NULL;
    s_eatable = NULL;
    d_eatable = NULL;

    score = 0;
    done = 0;
}

Game::~Game()
{
    // Delete content here

    delete snake;
    delete s_eatable;
    delete d_eatable;

    if(background != NULL)
        SDL_FreeSurface(background);

    if(snake_t != NULL)
        SDL_FreeSurface(snake_t);

    if(eatable != NULL)
        SDL_FreeSurface(eatable);

    if(dyn_eatable != NULL)
        SDL_FreeSurface(dyn_eatable);

    if(score_text != NULL)
        SDL_FreeSurface(score_text);

    if(end_text != NULL)
        SDL_FreeSurface(end_text);

    if(font != NULL)
        TTF_CloseFont(font);
}

bool Game::Init(){

    if(LoadContent() == false)
        return false;

    snake = new Snake(30, 30, 3);

    s_eatable = new Eatable(e_static);

    return true;
}

bool Game::Execute(SDL_Surface *screen, bool useAi, bool useSmartAi)
{
    int end = 0;
    Uint32 waittime = 1000.0f/FPS;
    Uint32 framestarttime = 0;
    Sint32 delaytime;
    Ai ai;
    SmartAi smartai;

	while(!done)
	{
	    if(useAi == false && useSmartAi == false)
            GetInput();
        else if(useSmartAi)
            GetAi(&smartai);
        else{
            GetAi(&ai);
        }

		end = Update();
		Render(screen, end);

        delaytime = waittime - (SDL_GetTicks() - framestarttime);
        if(delaytime > 0)
            SDL_Delay((Uint32)delaytime);
        framestarttime = SDL_GetTicks();
	}

    if(done == DIED)
        SDL_Delay(2000);

    return true;
}

// Private

int Game::LoadContent(){

    //Open the font
    font = TTF_OpenFont( "fonts/DigitalDreamFat.ttf", 24 );

     //If there was an error in loading the font
    if( font == NULL )
         return false;

    background = load_image( "img/g_bg.png" );

    if( background == NULL )
        return false;

    snake_t = load_image( "img/snake.png" );

    if( snake_t == NULL )
        return false;

    eatable = load_image( "img/eatable.png" );

    if( eatable == NULL )
        return false;

    dyn_eatable = load_image( "img/dyn_eatable.png" );

    if( dyn_eatable == NULL )
        return false;

    return true;
}

void Game::GetInput()
{
    int dirGot = 0;

	while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            done = QUITED;

        // Accept only one direction change when
        // GetInput is called so we won't crash
        // to itself in one move.
        if(dirGot)
            continue;

        //If a key was pressed
        if( event.type == SDL_KEYDOWN ) {

            //Set the proper message surface
            switch( event.key.keysym.sym )
            {
                case SDLK_UP: snake->SetDir(s_up); dirGot = 1; break;
                case SDLK_DOWN: snake->SetDir(s_down); dirGot = 1; break;
                case SDLK_LEFT: snake->SetDir(s_left); dirGot = 1; break;
                case SDLK_RIGHT: snake->SetDir(s_right); dirGot = 1; break;
                case SDLK_ESCAPE: done = QUITED; break;
                default: break;
            }
        }
    }
}

void Game::GetAi(Ai *ai)
{
    // Chase dynamic eatable if
    // it exists, and if not,
    // go for the basic eatable.
    if(d_eatable != NULL)
        ai->GetDir(snake, d_eatable);
    else
        ai->GetDir(snake, s_eatable);

    // Poll keys if user want's to quit
    while(SDL_PollEvent(&event)){

        if(event.type == SDL_QUIT){
            done = QUITED;
        }else if( event.type == SDL_KEYDOWN ) {

            //Set the proper message surface
            if( event.key.keysym.sym == SDLK_ESCAPE )
                done = QUITED;
        }
    }
}

void Game::GetAi(SmartAi *ai)
{
    // Chase dynamic eatable if
    // it exists, and if not,
    // go for the basic eatable.
    if(d_eatable != NULL)
        ai->GetDir(snake, d_eatable);
    else
        ai->GetDir(snake, s_eatable);

    // Poll keys if user want's to quit
    while(SDL_PollEvent(&event)){

        if(event.type == SDL_QUIT){
            done = QUITED;
        }else if( event.type == SDL_KEYDOWN ) {

            //Set the proper message surface
            if( event.key.keysym.sym == SDLK_ESCAPE )
                done = QUITED;
        }
    }
}

int Game::Update()
{
    int i;

    snake->Move();

    // Check that if snake has contact with itself
    for(i=1; i<snake->GetLength(); i++){

       if((snake->GetPosX(0) == snake->GetPosX(i)) && \
       (snake->GetPosY(0) == snake->GetPosY(i))){
            done = DIED;
            return 1;
       }
    }

    // Check if snake has collided with the eatable
    if((snake->GetPosX(0) == s_eatable->GetPosX(0)) && \
       (snake->GetPosY(0) == s_eatable->GetPosY(0))){

        score++;

        snake->Grow(1);

        // Delete eaten "eatable" and
        // create a new one.
        delete s_eatable;
        s_eatable = new Eatable(e_static);
    }

    if(d_eatable == NULL){

        if((rand() % 1000) < 5)
            d_eatable = new Eatable(e_dynamic);

    }else{

        // Check if snake has collided with the dynamic eatable
        if((snake->GetPosX(0) == d_eatable->GetPosX(0)) && \
           (snake->GetPosY(0) == d_eatable->GetPosY(0))){

            score += d_eatable->GetFrame() + 1;

            snake->Grow(1);

            delete d_eatable;
            d_eatable = NULL;
        }

        // Update dynamic eatable status
        // and check if it's still alive
        if(d_eatable != NULL){
            if(d_eatable->Update() == false){

                delete d_eatable;
                d_eatable = NULL;
            }
        }
    }

    return 0;
}

void Game::Render(SDL_Surface *screen, int end)
{
    int i;
    char score_array[20];
    SDL_Color textColor = { 230, 230, 230 };
    SDL_Rect clip[5];

    //Draw background to the screen
    apply_surface( 0, 0, background, screen );

    // Draw score to the screen
    strncpy(score_array, (char*)"SCORE: ", 7);
    itoa(score, &score_array[7], 10);
    score_text = TTF_RenderText_Solid( font, score_array, textColor );
    apply_surface( 5, 5, score_text, screen );

    //Draw snake to the screen
    clip[ 0 ].x = 0;
    clip[ 0 ].y = 0;
    clip[ 0 ].w = 10;
    clip[ 0 ].h = 10;

    clip[ 1 ].x = 10;
    clip[ 1 ].y = 0;
    clip[ 1 ].w = 10;
    clip[ 1 ].h = 10;

    apply_surface( snake->GetPosX(0), snake->GetPosY(0), snake_t, screen, &clip[0] );

    for(i=1; i<snake->GetLength(); i++)
        apply_surface( snake->GetPosX(i), snake->GetPosY(i), snake_t, screen, &clip[1] );

    //Draw eatable(s)
    apply_surface( s_eatable->GetPosX(0), s_eatable->GetPosY(0), eatable, screen );

    if(d_eatable != NULL){

        clip[ 0 ].x = 40;
        clip[ 0 ].y = 0;
        clip[ 0 ].w = 10;
        clip[ 0 ].h = 10;

        clip[ 1 ].x = 30;
        clip[ 1 ].y = 0;
        clip[ 1 ].w = 10;
        clip[ 1 ].h = 10;

        clip[ 2 ].x = 20;
        clip[ 2 ].y = 0;
        clip[ 2 ].w = 10;
        clip[ 2 ].h = 10;

        clip[ 3 ].x = 10;
        clip[ 3 ].y = 0;
        clip[ 3 ].w = 10;
        clip[ 3 ].h = 10;

        clip[ 4 ].x = 0;
        clip[ 4 ].y = 0;
        clip[ 4 ].w = 10;
        clip[ 4 ].h = 10;

        apply_surface( d_eatable->GetPosX(0), d_eatable->GetPosY(0), dyn_eatable, screen, &clip[d_eatable->GetFrame()]);
    }

    if(end){
        end_text = TTF_RenderText_Solid( font, (char*)"GAME OVER!", textColor );
        apply_surface( (SCREEN_WIDTH - end_text->w) / 2 , (SCREEN_HEIGHT - end_text->h) / 2, end_text, screen );

        SDL_FreeSurface(end_text);
        end_text = NULL;
    }

    SDL_FreeSurface(score_text);
    score_text = NULL;

    //Update the screen
    SDL_Flip( screen );
}
