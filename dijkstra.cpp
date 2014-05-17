#include "dijkstra.h"

using namespace std;

#define MAX 1000

vector<int> dijkstra(int matrix_size, int source, int target, int **cost)
{
    int i, u = 0, count, w, min, *dist;
    bool *flag, done = false;
    vector<int> previous_step(matrix_size);

    dist = new int[matrix_size];
    flag = new bool[matrix_size];

    for(i=0; i<matrix_size; i++)
    {
        flag[i] = false;
        dist[i] = cost[source][i];
    }

    count = 1;

    while(count < matrix_size && !done)
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

        flag[u] = true;
        count++;

        for(w=0; w<matrix_size; w++)
        {
            if((dist[u] + cost[u][w]<dist[w]) && !flag[w])
            {
                dist[w] = dist[u] + cost[u][w];
                previous_step.at(w) = u;

                if(u == target)
                    done = true;
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

    delete [] flag;
    delete [] dist;

    return path;
}
