#include "snake.h"
#include "defines.h"
#include <string.h>

Snake::Snake(int x, int y, int len) : Entity(MAX_LENGTH)
{
    int i;

    // Set head position

    pos[0]->x = x;
    pos[0]->y = y;

    for(i=1; i<len; i++){
        pos[i]->x = SCREEN_WIDTH;
        pos[i]->y = 0;
    }

    dir = right;
    length = len;
    speed = 1;
    pause = 0;
}

Snake::~Snake()
{

}

void Snake::SetDir(Direction n_dir){

    if(dir == left && n_dir == right)
        return;
    else if(dir == right && n_dir == left)
        return;
    else if(dir == up && n_dir == down)
        return;
    else if(dir == down && n_dir == up)
        return;

    dir = n_dir;
}

int Snake::GetLength(){

    return length;
}

Direction Snake::GetDir(){

    return dir;
}

void Snake::Grow(int bits){

    int i, o_length;

    o_length = length;

    if((length + bits) > MAX_LENGTH){
        length = MAX_LENGTH;
        bits = (MAX_LENGTH - o_length);
    }else{
        length += bits;
    }

    for(i=0; i<bits; i++)
        *pos[o_length + i] = *pos[o_length - 1];
}

void Snake::Move(){

    int i;

    if(pause++ < SNAKE_PAUSE)
        return;
    else
        pause = 0;

    for(i=length-1; i>0; i--){
        *pos[i] = *pos[i-1];
    }

    switch(dir){

        case right:

            if((pos[0]->x + GRID_SIZE * speed) < X_AREA)
                pos[0]->x += GRID_SIZE * speed;
            else
                pos[0]->x = 0;

            break;

        case down:

            if((pos[0]->y + GRID_SIZE * speed) < Y_AREA)
                pos[0]->y += GRID_SIZE * speed;
            else
                pos[0]->y = 0;

            break;

        case left:

            if((pos[0]->x - GRID_SIZE * speed) >= 0)
                pos[0]->x -= GRID_SIZE * speed;
            else
                pos[0]->x = X_AREA - GRID_SIZE;

            break;

        case up:

            if((pos[0]->y - GRID_SIZE * speed) >= 0)
                pos[0]->y -= GRID_SIZE * speed;
            else
                pos[0]->y = Y_AREA - GRID_SIZE;

            break;
    }
}
