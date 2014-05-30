#include "dijkstra.h"

using namespace std;

Dijkstra::Dijkstra(unsigned int m_size) {

	matrix_size = m_size;
	dist = new int[matrix_size];
	flag = new bool[matrix_size];
	previous_step.resize(matrix_size, -1);
}

Dijkstra::~Dijkstra() {

	delete [] flag;
	delete [] dist;
}

vector<int> Dijkstra::GetPath(int source, int target, vector_vertex_vector cost) {

	int i, u = 0, count, w, min;
	bool done = false;
	vector<int> path;

	for(i=0; i<matrix_size; i++) {
		flag[i] = false;
		dist[i] = INFINITE;
	}

    for(vertex_vector::iterator it = cost.at(source).begin(); it != cost.at(source).end(); ++it) {

        vertex node = (*it);

        dist[node.first] = node.second;
    }

	count = 1;

	previous_step.at(source) = 0;

	while(count < matrix_size && !done) {
		min = INFINITE;

		for(w=0; w<matrix_size; w++) {
			if(dist[w] < min && !flag[w]) {
				min = dist[w];
				u = w;
			}
		}

		flag[u] = true;
		count++;

		for(std::vector<vertex>::iterator it = cost.at(u).begin(); it != cost.at(u).end(); ++it) {

			vertex v = (*it);

			if((dist[u] + v.second < dist[v.first]) && !flag[v.first]) {
				dist[v.first] = dist[u] + v.second;
				previous_step.at(v.first) = u;

				if(u == target) {
					done = true;
					break;
				}
			}
		}
	}

	if(done) {
		int p_ptr = previous_step.at(target);

		path.push_back(target);

		while(p_ptr != -1) {
			path.push_back(p_ptr);
			p_ptr = previous_step.at(p_ptr);
		}
	}

	previous_step.resize(0);
	previous_step.resize(matrix_size, -1);

	return path;
}
