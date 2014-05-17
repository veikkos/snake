#include "dijkstra.h"

using namespace std;

#define MAX 1000

vector<int> dijkstra(int matrix_size, int source, int target, int **cost, int *dist)
{
    int i, u, count, w, *flag, min;

    vector<int> previous_step(matrix_size);

    flag = new int[matrix_size];

    for(i=0; i<matrix_size; i++)
    {
        flag[i] = 0;
        dist[i] = cost[source][i];
    }

    count = 1;

    while(count < matrix_size)
    {
        min = MAX;

        for(w=0; w<matrix_size; w++)
        {
           if(dist[w] < min && !flag[w])
           {
               min = dist[w];
               u = w;
           }
        }

        flag[u] = 1;
        count++;

        for(w=0; w<matrix_size; w++)
        {
           if((dist[u] + cost[u][w]<dist[w]) && !flag[w])
           {
              dist[w] = dist[u] + cost[u][w];
              previous_step.at(w) = u;
           }
        }
    }

    vector<int> path;
    int p_ptr = previous_step.at(target);

    path.push_back(target);

    while(p_ptr && previous_step.at(p_ptr) != 0){
        path.push_back(p_ptr);
        p_ptr = previous_step.at(p_ptr);
    }

    if(p_ptr)
        path.push_back(p_ptr);

    delete[] flag;

    return path;
}
