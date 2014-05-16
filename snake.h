#ifndef _SNAKE_H_
#define _SNAKE_H_
#include "defines.h"
#include "entity.h"

typedef enum{

    right = 0,
    down,
    left,
    up

} Direction;

class Snake : public Entity
{
public:
    Snake(int x, int y, int len);
    ~Snake();

    void SetDir(Direction n_dir);
    int GetLength();
    Direction GetDir();
    void Grow(int bits);

    void Move();

private:

    Direction dir;
    int speed;
    int pause;
    int length;
};


#endif
