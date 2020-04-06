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
  vector_vertex_vector* cost) {

  bool done = false;
  vector<int> path;
  std::vector<int> next_indexes;
  next_indexes.reserve(matrix_size / 16);
  int last_min = INFINITE;

  for (int i = 0; i < matrix_size; i++) {
    dist[i].second = false;
    dist[i].first = INFINITE;
  }

  dist[source].first = 0;

  previous_step.at(source) = -1;

  next_indexes.emplace_back(source);

  while (!next_indexes.empty() && !done) {

    int min = INFINITE;
    int lowest_dist_index;

    for (size_t i = 0; i < next_indexes.size(); ++i) {

      const int& dist_index = next_indexes[i];
      const int& dist_value = dist[dist_index].first;

      if (dist_value < min) {
        lowest_dist_index = i;
        min = dist_value;
        if (min == last_min)
          break;
      }
    }

    last_min = min;

    const int dist_index = next_indexes[lowest_dist_index];
    next_indexes.erase(next_indexes.begin() + lowest_dist_index);
    dist[dist_index].second = true;

    const vertex_vector& vertex_vector = cost->at(dist_index);

    for (size_t i = 0; i < vertex_vector.size(); ++i) {

      const vertex& cost_vertex = vertex_vector[i];
      const int& cost_pos = cost_vertex.first;
      const int cost_value = dist[dist_index].first + cost_vertex.second;
      dist_pair& dist_cos = dist[cost_pos];

      if ((cost_value < dist_cos.first) &&
        !dist_cos.second) {
        dist_cos.first = cost_value;
        previous_step.at(cost_pos) = dist_index;
        next_indexes.emplace_back(cost_pos);

        if (dist_index == target || cost_pos == target) {
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
