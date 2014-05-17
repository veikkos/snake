#ifndef _SMART_AI_H_
#define _SMART_AI_H_

#include "snake.h"
#include "eatables.h"
#include "dijkstra.h"
#include "defines.h"

class SmartAi{

public:
    SmartAi();
    ~SmartAi();

    void GetDir(Snake *ai_snake, Eatable *cur_eatable);

private:
    bool BlockHasSnake(Snake *snake, int x, int y);
};

#endif
