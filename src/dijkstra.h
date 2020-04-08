#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <vector>

class Dijkstra {

public:
  Dijkstra(unsigned int matrix_size);
  ~Dijkstra();

  struct Cost { int values[4]; };

  std::vector<int> GetPath(int source, int target, Cost *cost);

private:

  struct Dist { int value; bool done; int previous; };
  Dist *dist;
  int matrix_size;
};

#endif // DIJKSTRA_H_
