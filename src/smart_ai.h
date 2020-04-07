#ifndef _SMART_AI_H_
#define _SMART_AI_H_

#include "snake.h"
#include "eatables.h"
#include "dijkstra.h"
#include "defines.h"
#include <vector>

class SmartAi {

public:
  SmartAi();
  ~SmartAi();

  void GetDir(Snake *ai_snake, Eatable *cur_eatable);
  std::vector< std::pair <int, int> > GetPath();

private:

  bool CollisionIn(Snake *ai_snake, Direction dir);
  bool BlockHasSnake(Snake *snake, int x, int y);
  bool BlockHasSnake(Snake *snake, int block);
  void NoPath(Snake *ai_snake, Eatable *cur_eatable);

  static void GenerateGrid(int matrix_size, Dijkstra::Cost* cost);

  int m_size;
  Dijkstra::Cost* cost_grid;
  Dijkstra::Cost* cost;
  Dijkstra *dijkstra;

  std::vector<int> path;
};

#endif
