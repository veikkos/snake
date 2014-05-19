#include "smart_ai.h"
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

SmartAi::SmartAi(){

    int i;
    m_size = (X_AREA / GRID_SIZE) * (Y_AREA / GRID_SIZE);
    cost = new int*[m_size];
    cost_grid = new int*[m_size];

    for(i = 0; i < m_size; i++)
    {
        cost[i] = new int[m_size];
        cost_grid[i] = new int[m_size];
    }

    GenerateGrid(m_size, X_AREA / GRID_SIZE, cost_grid);

    dijkstra = new Dijkstra(m_size);
}

SmartAi::~SmartAi(){

    int i;

    for(i=0; i < m_size; i++)
    {
        delete [] cost[i];
        delete [] cost_grid[i];
    }

    delete [] cost;
    delete [] cost_grid;

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

    int i;
    int x_blocks = X_AREA / GRID_SIZE;
    int snake_head, target;
    vector<int> path;

    CopyGrid(cost, cost_grid, m_size);

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

    if((next_block - snake_head == 1) || \
        ((next_block < snake_head) && (snake_head - next_block == x_blocks - 1)))
        ai_snake->SetDir(s_right);
    else if((snake_head - next_block == 1) || \
        ((next_block > snake_head) && (next_block - snake_head == x_blocks - 1)))
        ai_snake->SetDir(s_left);
    else if((snake_head - next_block == x_blocks) || \
        ((next_block > snake_head) && (snake_head < x_blocks) && (next_block >= m_size - x_blocks)))
        ai_snake->SetDir(s_up);
    else if((next_block - snake_head == x_blocks) || \
        ((next_block < snake_head) && (next_block < x_blocks) && (snake_head >= m_size - x_blocks)))
        ai_snake->SetDir(s_down);
}

void SmartAi::GenerateGrid(int matrix_size, int width, int **cost_matrix){

    int x, y;

    for(y = 0; y < matrix_size; y++)
    {
        for(x = 0; x < matrix_size; x++)
        {
            if(y == x){
                // Move to itself is always 0
                cost_matrix[y][x] = 0;
            }else if((abs(y-x) == 1) && \
                    !(!(x % width) && x > y) && \
                    !(!(y % width) && x < y)){
                // Move to adjacent (but not to different row)
                cost_matrix[y][x] = 1;
            }else if(abs(x - y) == width){
                // Move to up or down
                cost_matrix[y][x] = 1;
            }else if(((x < width && y >= matrix_size - width) || \
                    (y < width && x >= matrix_size - width)) && \
                    ((y % width) == (x % width))){
                // Top to bottom and vice versa
                cost_matrix[y][x] = 1;
            }else if( (!(x % width) && (y == x + width - 1)) ||
                    (!(y % width) && (x == y + width - 1))){
                // Left to right and vice versa
                cost_matrix[y][x] = 1;
            }else{
                // To any other is infinite
                cost_matrix[y][x] = INFINITE;
            }
        }
    }
}

void SmartAi::CopyGrid(int **dst, int **src, int size){

    int x, y;

    for(x = 0; x < m_size; x++){
        for(y = 0; y < m_size; y++)
        {
            dst[x][y] = src[x][y];
        }
    }
}
