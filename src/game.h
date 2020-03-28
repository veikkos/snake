#ifndef _GAME_H_
#define _GAME_H_

#include <stdlib.h>
#include <vector>

#include "snake.h"
#include "eatables.h"
#include "ai.h"
#include "smart_ai.h"
#include "port.h"

#define QUITED  1
#define DIED    2

class Game {
public:
  Game();
  ~Game();

  bool Init(Handle handle);

  typedef enum {
    SINGLE,
    AI,
    SMART_AI
  } game_mode;

  bool Execute(Handle handle, game_mode mode);

private:

  int score;
  bool framelimit;
  bool render_path;

  int LoadContent(Handle handle);

  void GetInput();

  template <class TYPE>
  void GetAi(TYPE *ai);

  int Update();
  void Render(Handle handle, int end);

  int done;

  Image background;
  Image snake_t;
  Image eatable;
  Image dyn_eatable;
  Image score_text;
  Image end_text;
  Image path_mark;

  Font font;

  Snake *snake;
  Eatable *s_eatable;
  Eatable *d_eatable;

  Ai *ai;
  SmartAi *smartai;
};


#endif
