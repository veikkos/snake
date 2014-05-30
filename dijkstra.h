#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <vector>

#define INFINITE 1000

class Dijkstra {

public:
	Dijkstra(unsigned int matrix_size);
	~Dijkstra();

    typedef std::pair <int,int> vertex;
    typedef std::vector<vertex> vertex_vector;
    typedef std::vector<vertex_vector> vector_vertex_vector;

	std::vector<int> GetPath(int source, int target, vector_vertex_vector cost);

private:
	int *dist;
	bool *flag;
	int matrix_size;
	std::vector<int> previous_step;
};

#endif // DIJKSTRA_H_
