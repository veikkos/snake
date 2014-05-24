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

    bool CollisionIn(Snake *ai_snake, Direction dir);
    bool BlockHasSnake(Snake *snake, int x, int y);
    bool BlockHasSnake(Snake *snake, int block);
    void NoPath(Snake *ai_snake, Eatable *cur_eatable);

    void GenerateGrid(int matrix_size, int width, int **cost);
    static void CopyGrid(int **dst, int **src, int size);

    int m_size;
    int **cost_grid;
    int **cost;
    Dijkstra *dijkstra;
};

#endif
