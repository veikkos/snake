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

bool SmartAi::BlockHasSnake(Snake *snake, int block){

    int x, y;

    y = block / X_AREA;
    x = block % X_AREA;

    return BlockHasSnake(snake, x, y);
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

    // If path exist, go for the target
    if(path.size()){

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

    }else{
        // If path isn't found, avoid colliding by itself

        NoPath(ai_snake, cur_eatable);
    }
}

void SmartAi::NoPath(Snake *ai_snake, Eatable *cur_eatable){

    Direction new_dir = ai_snake->GetDir();

    if(CollisionIn(ai_snake, new_dir) == false){
        ai_snake->SetDir(new_dir);
    }else{
        // Prioritize the first two options with the distance to
        // the eatable.
        switch(new_dir){

            case s_right:
            {
                if(ai_snake->GetPosY(0) > cur_eatable->GetPosY(0)){
                    if(CollisionIn(ai_snake, s_up) == false){
                        ai_snake->SetDir(s_up);
                        return;
                    }else if(CollisionIn(ai_snake, s_down) == false){
                        ai_snake->SetDir(s_down);
                        return;
                    }
                }else{
                    if(CollisionIn(ai_snake, s_down) == false){
                        ai_snake->SetDir(s_down);
                        return;
                    }else if(CollisionIn(ai_snake, s_up) == false){
                        ai_snake->SetDir(s_up);
                        return;
                    }
                }

                if(CollisionIn(ai_snake, s_left) == false){
                    ai_snake->SetDir(s_left);
                    return;
                }

                break;
            }
            case s_left:
            {
                if(ai_snake->GetPosY(0) > cur_eatable->GetPosY(0)){
                    if(CollisionIn(ai_snake, s_up) == false){
                        ai_snake->SetDir(s_up);
                        return;
                    }else if(CollisionIn(ai_snake, s_down) == false){
                        ai_snake->SetDir(s_down);
                        return;
                    }
                }else{
                    if(CollisionIn(ai_snake, s_down) == false){
                        ai_snake->SetDir(s_down);
                        return;
                    }else if(CollisionIn(ai_snake, s_up) == false){
                        ai_snake->SetDir(s_up);
                        return;
                    }
                }

                if(CollisionIn(ai_snake, s_right) == false){
                    ai_snake->SetDir(s_right);
                    return;
                }

                break;
            }
            case s_down:
            {
                if(ai_snake->GetPosX(0) > cur_eatable->GetPosX(0)){
                    if(CollisionIn(ai_snake, s_left) == false){
                        ai_snake->SetDir(s_left);
                        return;
                    }else if(CollisionIn(ai_snake, s_right) == false){
                        ai_snake->SetDir(s_right);
                        return;
                    }
                }else{
                    if(CollisionIn(ai_snake, s_right) == false){
                        ai_snake->SetDir(s_right);
                        return;
                    }else if(CollisionIn(ai_snake, s_left) == false){
                        ai_snake->SetDir(s_left);
                        return;
                    }
                }

                if(CollisionIn(ai_snake, s_up) == false){
                    ai_snake->SetDir(s_up);
                    return;
                }

                break;
            }
            case s_up:
            {
                if(ai_snake->GetPosX(0) > cur_eatable->GetPosX(0)){
                    if(CollisionIn(ai_snake, s_left) == false){
                        ai_snake->SetDir(s_left);
                        return;
                    }else if(CollisionIn(ai_snake, s_right) == false){
                        ai_snake->SetDir(s_right);
                        return;
                    }
                }else{
                    if(CollisionIn(ai_snake, s_right) == false){
                        ai_snake->SetDir(s_right);
                        return;
                    }else if(CollisionIn(ai_snake, s_left) == false){
                        ai_snake->SetDir(s_left);
                        return;
                    }
                }

                if(CollisionIn(ai_snake, s_down) == false){
                    ai_snake->SetDir(s_down);
                    return;
                }

                break;
            }
        }
    }
}

bool SmartAi::CollisionIn(Snake *ai_snake, Direction dir){

    int i;
    int x, y;

    for(i=1; i<ai_snake->GetLength(); i++){

        switch(dir){

            case s_right:
            {
                x = ai_snake->GetPosX(0) + GRID_SIZE;
                y = ai_snake->GetPosY(0);

                if(x >= X_AREA)
                    x -= X_AREA;

                if(x == ai_snake->GetPosX(i)){

                    if(y == ai_snake->GetPosY(i)){
                        return true;
                    }
                }

                break;
            }
            case s_left:
            {
                x = ai_snake->GetPosX(0) - GRID_SIZE;
                y = ai_snake->GetPosY(0);

                if(x <= 0)
                    x += X_AREA;

                if(x == ai_snake->GetPosX(i)){

                    if(y == ai_snake->GetPosY(i)){
                        return true;
                    }
                }

                break;
            }
            case s_down:
            {
                y = ai_snake->GetPosY(0) + GRID_SIZE;
                x = ai_snake->GetPosX(0);

                if(y >= Y_AREA)
                    y -= Y_AREA;

                if(y == ai_snake->GetPosY(i)){

                    if(x == ai_snake->GetPosX(i)){
                        return true;
                    }
                }

                break;
            }
            case s_up:
            {
                y = ai_snake->GetPosY(0) - GRID_SIZE;
                x = ai_snake->GetPosX(0);

                if(y <= 0)
                    y += Y_AREA;

                if(y == ai_snake->GetPosY(i)){

                    if(x == ai_snake->GetPosX(i)){
                        return true;
                    }
                }

                break;
            }
        }
    }

    return false;
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

    for(x = 0; x < size; x++){
        for(y = 0; y < size; y++)
        {
            dst[x][y] = src[x][y];
        }
    }
}
