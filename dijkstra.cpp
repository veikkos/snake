#include "dijkstra.h"

#define MAX 1000

void dijkstra(int matrix_size, int v, int **cost, int *dist)
{
    int i, u, count, w, *flag, min;

    flag = new int[matrix_size];

    for(i=0; i<matrix_size; i++)
    {
        flag[i] = 0;
        dist[i] = cost[v][i];
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
           }
        }
    }

    delete[] flag;
}
