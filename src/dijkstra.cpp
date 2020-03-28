#include "dijkstra.h"

using namespace std;

Dijkstra::Dijkstra(unsigned int m_size) {

  matrix_size = m_size;
  dist = new dist_pair[matrix_size];
  previous_step.resize(matrix_size, -1);
}

Dijkstra::~Dijkstra() {

  delete[] dist;
}

vector<int> Dijkstra::GetPath(int source, int target,
  vector_vertex_vector cost) {

  int i, u = 0, count, w, min;
  bool done = false;
  vector<int> path;

  for (i = 0; i < matrix_size; i++) {
    dist[i].second = false;
    dist[i].first = INFINITE;
  }

  dist[source].first = 0;

  count = 1;

  previous_step.at(source) = -1;

  while (count < matrix_size && !done) {
    min = INFINITE;

    for (w = 0; w < matrix_size; w++) {
      if (dist[w].first < min && !dist[w].second) {
        min = dist[w].first;
        u = w;
      }
    }

    dist[u].second = true;
    count++;

    for (std::vector<vertex>::iterator it = cost.at(u).begin();
      it != cost.at(u).end(); ++it) {

      vertex v = (*it);

      if ((dist[u].first + v.second < dist[v.first].first) &&
        !dist[v.first].second) {
        dist[v.first].first = dist[u].first + v.second;
        previous_step.at(v.first) = u;

        if (u == target || v.first == target) {
          done = true;
          break;
        }
      }
    }
  }

  if (done) {
    int p_ptr = previous_step.at(target);

    path.push_back(target);

    while (p_ptr != -1 && p_ptr != source) {
      path.push_back(p_ptr);
      p_ptr = previous_step.at(p_ptr);
    }
  }

  previous_step.resize(0);
  previous_step.resize(matrix_size, -1);

  return path;
}
