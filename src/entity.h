#ifndef _ENTITY_H_
#define _ENTITY_H_

typedef struct {
  int x;
  int y;
} position;

class Entity {
public:
  Entity(int size);
  ~Entity();

  int GetPosY(int bit);
  int GetPosX(int bit);
  position GetPos(int bit);

  position **pos;
  int size;

private:
  int pos_alloc_size;
};


#endif
