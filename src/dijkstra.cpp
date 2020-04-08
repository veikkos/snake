#include "dijkstra.h"
#include <climits>

Dijkstra::Dijkstra(unsigned int m_size) {

  matrix_size = m_size;
  dist = new Dist[matrix_size];
}

Dijkstra::~Dijkstra() {

  delete[] dist;
}

std::vector<int> Dijkstra::GetPath(int source, int target, Cost *cost) {

  bool done = false;
  std::vector<int> path;
  std::vector<int> next_indexes;
  int last_min = INT_MAX;

  path.reserve(matrix_size / 32);
  next_indexes.reserve(matrix_size / 16);

  for (int i = 0; i < matrix_size; i++) {
    dist[i].done = false;
    dist[i].value = INT_MAX;
    dist[i].previous = -1;
  }

  dist[source].value = 0;

  next_indexes.emplace_back(source);

  while (!next_indexes.empty() && !done) {

    int min = INT_MAX;
    int lowest_dist_index;

    for (size_t i = 0; i < next_indexes.size(); ++i) {

      const int& dist_index = next_indexes[i];
      const int& dist_value = dist[dist_index].value;

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
    dist[dist_index].done = true;
    const int value = dist[dist_index].value + 1;

    for (size_t i = 0; i < sizeof(Cost) / sizeof(Cost::values[0]); ++i) {

      const int& cost_pos = cost[dist_index].values[i];
      if (cost_pos != -1) {
        Dist& dist_cos = dist[cost_pos];

        if ((value < dist_cos.value) && !dist_cos.done) {
          dist_cos.value = value;
          dist[cost_pos].previous = dist_index;
          next_indexes.emplace_back(cost_pos);

          if (dist_index == target || cost_pos == target) {
            done = true;
            break;
          }
        }
      }
    }
  }

  if (done) {
    int p_ptr = dist[target].previous;

    path.push_back(target);

    while (p_ptr != -1 && p_ptr != source) {
      path.push_back(p_ptr);
      p_ptr = dist[p_ptr].previous;
    }
  }

  return path;
}
