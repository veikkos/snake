#ifndef _GAME_H_
#define _GAME_H_

#include <stdlib.h>
#include <vector>

#include "defines.h"
#include "snake.h"
#include "eatables.h"
#include "ai.h"
#include "smart_ai.h"
#include "port.h"
#include "popup.h"

#define QUITED  1
#define DIED    2

class Game {
public:
  Game();
  ~Game();

  bool Init(PortHandle handle);

  typedef enum {
    SINGLE,
    AI,
    SMART_AI
  } game_mode;

  bool Execute(PortHandle handle, game_mode mode);

private:

  int score;
  bool framelimit;
  bool render_path;
  int since_last_eat;

  int LoadContent(PortHandle handle);

  void GetInput();

  template <class TYPE>
  void GetAi(TYPE *ai);

  int Update(PortHandle handle);
  bool UpdateEatables(PortHandle handle);
  void UpdatePopups();

  void Render(PortHandle handle, int end);
  void RenderSnake(PortHandle handle);
  void RenderEatables(PortHandle handle);
  void RenderPopups(PortHandle handle);
  void RenderScore(PortHandle handle);
  void RenderPath(PortHandle handle);
  void RenderGameOver(PortHandle handle);

  void InactivityBlocker(PortHandle handle, bool ate);

  int done;

  Image background;
  Image snake_t;
  Image eatable;
  Image dyn_eatable;
  Image dyn_eatable_2;
  Image path_mark;

  Font font;
  Font font_s;

  Snake *snake;
  Eatable *s_eatable;
  Eatable *d_eatable;

  Ai *ai;
  SmartAi *smartai;

  std::vector<Popup*> popups;
};


#endif
