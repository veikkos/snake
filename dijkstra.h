#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <vector>

#define INFINITE 1000

class Dijkstra {

public:
	Dijkstra(unsigned int matrix_size);
	~Dijkstra();

	std::vector<int> GetPath(int source, int target, int **cost);

private:
	int *dist;
	bool *flag;
	int matrix_size;
	std::vector<int> previous_step;
};

#endif // DIJKSTRA_H_
