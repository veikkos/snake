#include <stdlib.h>
#include "entity.h"

Entity::Entity(int n_size)
{
    int i;

    pos = new position*[n_size];

    for(i=0; i<n_size; i++){
        pos[i] = new position;
        pos[i]->x = 0;
        pos[i]->y = 0;
    }

    pos_alloc_size = n_size;
}

Entity::~Entity()
{
    int i;

    for(i=0; i<pos_alloc_size; i++)
        delete pos[i];

    delete [] pos;
}

int Entity::GetPosY(int bit){

    return pos[bit]->y;
}

int Entity::GetPosX(int bit){

    return pos[bit]->x;
}

position Entity::GetPos(int bit){

    return *pos[bit];
}
