#include "smart_ai.h"
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

SmartAi::SmartAi(){

    int i;
    m_size = (X_AREA / GRID_SIZE) * (Y_AREA / GRID_SIZE);
    cost = new int*[m_size];

    for(i = 0; i < m_size; i++)
    {
        cost[i] = new int[m_size];
    }

    dijkstra = new Dijkstra(m_size);
}

SmartAi::~SmartAi(){

    int i;

    for(i=0; i < m_size; i++)
    {
        delete [] cost[i];
    }

    delete [] cost;

    delete dijkstra;
}

bool SmartAi::BlockHasSnake(Snake *snake, int x, int y){

    int i;

    for(i=0; i < snake->GetLength(); i++){

        if(snake->GetPosX(i) / GRID_SIZE == x && snake->GetPosY(i) / GRID_SIZE == y)
            return true;
    }

    return false;
}

void SmartAi::GetDir(Snake *ai_snake, Eatable *cur_eatable){

    int x, y, i;
    int x_blocks = X_AREA / GRID_SIZE;
    int snake_head, target;
    vector<int> path;

    for(y = 0; y < m_size; y++)
    {
        for(x = 0; x < m_size; x++)
        {
            if(y == x){
                // Move to itself is always 0
                cost[y][x] = 0;
            }else if((abs(y-x) == 1) && \
                    !(!(x % x_blocks) && x > y) && \
                    !(!(y % x_blocks) && x < y)){
                // Move to adjacent (but not to different row)
                cost[y][x] = 1;
            }else if(abs(x - y) == x_blocks){
                // Move to up or down
                cost[y][x] = 1;
            }else if(((x < x_blocks && y >= m_size - x_blocks) || \
                    (y < x_blocks && x >= m_size - x_blocks)) && \
                    ((y % x_blocks) == (x % x_blocks))){
                // Top to bottom and vice versa
                cost[y][x] = 1;
            }else if( (!(x % x_blocks) && (y == x + x_blocks - 1)) ||
                    (!(y % x_blocks) && (x == y + x_blocks - 1))){
                // Left to right and vice versa
                cost[y][x] = 1;
            }else{
                // To any other is infinite
                cost[y][x] = INFINITE;
            }
        }
    }

    snake_head = (ai_snake->GetPosY(0) / GRID_SIZE) * \
                                x_blocks + ai_snake->GetPosX(0) / \
                                GRID_SIZE;

    for(i=1; i < ai_snake->GetLength(); i++){
        int snake_in_block = (ai_snake->GetPosY(i) / GRID_SIZE) * \
                                x_blocks + (ai_snake->GetPosX(i) / \
                                GRID_SIZE);

        if(snake_in_block != snake_head){
            int m_count;

            for(m_count = 0; m_count < m_size; m_count++){

                if(m_count != snake_in_block){
                    cost[snake_in_block][m_count] = INFINITE;
                    cost[m_count][snake_in_block] = INFINITE;
                }
            }
        }
    }

    target = cur_eatable->GetPosY(0) / GRID_SIZE;
    target *= x_blocks;
    target += cur_eatable->GetPosX(0) / GRID_SIZE;

    path = dijkstra->GetPath(snake_head, target, cost);

    int next_block = path.back();

    // TODO: Add special cases when going from top to bottom,
    // or vice versa.
    if((next_block - snake_head == 1) || \
        ((next_block < snake_head) && (snake_head - next_block == x_blocks - 1)))
        ai_snake->SetDir(s_right);
    else if((snake_head - next_block == 1) || \
        ((next_block > snake_head) && (next_block - snake_head == x_blocks - 1)))
        ai_snake->SetDir(s_left);
    else if(next_block < snake_head)
        ai_snake->SetDir(s_up);
    else if(next_block > snake_head)
        ai_snake->SetDir(s_down);
}
