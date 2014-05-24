#ifndef _GAME_H_
#define _GAME_H_

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include <stdlib.h>
#include <vector>

#include "screen.h"
#include "snake.h"
#include "eatables.h"
#include "ai.h"
#include "smart_ai.h"

#define QUITED  1
#define DIED    2

class Game
{
public:
    Game();
    ~Game();

    bool Init();
    bool Execute(SDL_Surface *screen, bool useAi = NULL, bool useSmartAi = NULL);

private:

    int score;
    bool framelimit;

    int LoadContent();

    void GetInput();

    template <class TYPE>
    void GetAi(TYPE *ai);

    int Update();
    void Render(SDL_Surface *screen, int end);

    int done;

    SDL_Event event;

    SDL_Surface *background;
    SDL_Surface *snake_t;
    SDL_Surface *eatable;
    SDL_Surface *dyn_eatable;
    SDL_Surface *score_text;
    SDL_Surface *end_text;

    TTF_Font *font;

    Snake *snake;
    Eatable *s_eatable;
    Eatable *d_eatable;
};


#endif
