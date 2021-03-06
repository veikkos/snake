#ifndef _EATABLE_H_
#define _EATABLE_H_

#include <vector>

#include "defines.h"
#include "entity.h"

typedef enum {

  e_static = 0,
  e_dynamic,
  e_dynamic_shrink

} eatable_type;

class Eatable : public Entity {
public:
  Eatable(eatable_type n_type, std::vector<position> *used);
  ~Eatable();

  bool Update();
  int GetFrame();
  eatable_type GetType() const;

private:

  eatable_type type;
  int frame;
  int tick;
  int alive;

  int nearest(int value, int near);
};


#endif
