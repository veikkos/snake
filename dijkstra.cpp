#include "dijkstra.h"

using namespace std;

Dijkstra::Dijkstra(unsigned int m_size){

    matrix_size = m_size;
    dist = new int[matrix_size];
    flag = new bool[matrix_size];
    previous_step.resize(matrix_size, -1);
}

Dijkstra::~Dijkstra(){

    delete [] flag;
    delete [] dist;
}

vector<int> Dijkstra::GetPath(int source, int target, int **cost)
{
    int i, u = 0, count, w, min;
    bool done = false;
    vector<int> path;

    for(i=0; i<matrix_size; i++)
    {
        flag[i] = false;
        dist[i] = cost[source][i];
    }

    count = 1;

    previous_step.at(source) = 0;

    while(count < matrix_size && !done)
    {
        min = INFINITE;

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

                if(u == target || w == target){
                    done = true;
                    break;
                }
            }
        }
    }

    if(done){
        int p_ptr = previous_step.at(target);

        path.push_back(target);

        while(p_ptr != -1){
            path.push_back(p_ptr);
            p_ptr = previous_step.at(p_ptr);
        }
    }

    previous_step.resize(0);
    previous_step.resize(matrix_size, -1);

    return path;
}
