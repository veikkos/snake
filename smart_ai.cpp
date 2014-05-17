#include "smart_ai.h"
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

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

    int i;

    int snake_head = (ai_snake->GetPosY(0) / GRID_SIZE) * \
                                x_blocks + ai_snake->GetPosX(0) / \
                                GRID_SIZE;

    for(i=1; i<ai_snake->GetLength(); i++){
        int snake_in_block = (ai_snake->GetPosY(i) / GRID_SIZE) * \
                                x_blocks + ai_snake->GetPosX(i) / \
                                GRID_SIZE;

        if(snake_in_block != snake_head){
            int m_count;

            for(m_count = 0; m_count < m_size; m_count++){

                if(m_count != snake_in_block){
                    cost[snake_in_block][m_count] = 1000;
                    cost[m_count][snake_in_block] = 1000;
                }
            }
        }
    }

    int target = cur_eatable->GetPosY(0) / GRID_SIZE;
    target *= x_blocks;
    target += cur_eatable->GetPosX(0) / GRID_SIZE;

    vector<int> path = dijkstra(m_size, snake_head, target, cost, dist);

    // TODO: return dir based on the shortest route

    for(y=0; y<m_size; y++)
    {
        delete[] cost[y];
    }

    delete[] cost;
}
