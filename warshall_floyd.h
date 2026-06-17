
//atcoder problems/073D.cpp
#define MAX_V 100

int d[MAX_V][MAX_V];    //d[u][v] is cost of e(u, v)
int V;      //num of v

for(int i = 0; i < V; i++){
    for(int j = 0; j < V; j++){
        if(i == j) d[i][j] = 0;
        else d[i][j] = INF;
    }
}

void warshall_floyd(){
    for(int k = 0; k < V; k++)
        for(int i = 0; i < V; i++)
            for(int j = 0; j < V; j++)
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
}
