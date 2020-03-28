#ifndef _AI_H_
#define _AI_H_

#include "snake.h"
#include "eatables.h"

class Ai {

public:
  Ai();
  ~Ai();

  void GetDir(Snake *ai_snake, Eatable *cur_eatable);

private:
  bool CollisionIn(Snake *ai_snake, Direction dir);
};

#endif
