#include "smart_ai.h"
#include <cmath>
#include <iostream>

SmartAi::SmartAi(){

}

SmartAi::~SmartAi(){

}

bool SmartAi::BlockHasSnake(Snake *snake, int x, int y){

    int i;

    for(i=0; i<snake->GetLength(); i++){

        if(snake->GetPosX(i) / GRID_SIZE == x && snake->GetPosY(i) / GRID_SIZE == y)
            return true;
    }

    return false;
}

void SmartAi::GetDir(Snake *ai_snake, Eatable *cur_eatable){

    int x, y, **cost, *dist;
    int m_size = (X_AREA / GRID_SIZE) * (Y_AREA / GRID_SIZE);
    int x_blocks = X_AREA / GRID_SIZE;
    int y_blocks = Y_AREA / GRID_SIZE;

    dist = new int[m_size];
    cost = new int*[m_size];

    for(y = 0; y < m_size; y++)
    {
        cost[y] = new int[m_size];

        for(x = 0; x < m_size; x++)
        {
            if(y == x)
                cost[y][x] = 0;
            if(BlockHasSnake(ai_snake, x, y))
                cost[y][x] = 1000;
            else if(std::abs(y-x) == 1)
                cost[y][x] = 1;
            else if(x - y == y_blocks)
                cost[y][x] = 1;
            else if(y - x == x_blocks)
                cost[y][x] = 1;
            else
                cost[y][x] = 1000;
        }
    }

    int source_node = ai_snake->GetPosY(0) / GRID_SIZE;
    source_node *= x_blocks;
    source_node += ai_snake->GetPosX(0) / GRID_SIZE;

    dijkstra(m_size, source_node, cost, dist);
    //dijkstra(m_size, cost, ai_snake->GetPosX(), ai_snake->GetPosY(), cur_eatable.x, cur_eatable.y);

    for(y=0; y<m_size; y++)
    {
        delete[] cost[y];
    }

    delete[] cost;
}
