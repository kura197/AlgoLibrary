#pragma once

#include <algorithm>
#include <climits>
#include <cstring>
#include <vector>

using namespace std;

//最大流    O(Ef), f:最大フロー

const int MAX_V = 101000;
const int INF = INT_MAX;

//{行き先、容量、逆辺}
struct edge {int to, cap, rev; };

vector<edge> G[MAX_V];   //隣接リスト
bool used[MAX_V];

//増加パスをDFSで探す
int dfs(int v, int t, int f){
    if(v == t) return f;
    used[v] = true;
    for(int i = 0; i < G[v].size(); i++){
        edge &e = G[v][i];
        if(!used[e.to] && e.cap > 0){
            int d = dfs(e.to, t, min(f, e.cap));
            if(d > 0){
                e.cap -= d;
                G[e.to][e.rev].cap += d;
                return d;
            }
        }
    }
    return 0;
}

//sからtへの最大流
int max_flow(int s, int t){
    int flow = 0;
    for(;;){
        memset(used, 0, sizeof(used));
        int f = dfs(s, t, INF);
        if(f == 0) return flow;
        flow += f;
    }
}


//call from main function
//fromからtoへの容量capの辺を追加
void add_edge(int from, int to, int cap){
    G[from].push_back((edge){to, cap, G[to].size()});
    G[to].push_back((edge){from, 0, G[from].size() - 1});
}
