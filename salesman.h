#pragma once


const long long INF = 1e15;
#define MAX_N 18

long long N;
long long D[MAX_N][MAX_N];

long long DP[1 << MAX_N][MAX_N];

// すべての頂点を一度ずつめぐり帰ってくる経路のうち、重みの総和の最小値を求める。
// O(2**n n**2)
long long rec(long long S, long long v){
    if(DP[S][v] >= 0){
        return DP[S][v];
    }

    if(S == (1 << N)-1 && v == 0)
        return DP[S][v] = 0;

    long long res = INF;
    for(int u = 0; u < N; u++){
        if(!(S >> u & 1)){
            res = min(res, rec(S | 1 << u, u) + D[v][u]);
        }
    }
    return DP[S][v] = res;
}

// in main
REP(i, 1<<MAX_N){
    REP(j,MAX_N){
        DP[i][j] = -1;
    }
}

long long ans = rec(0, 0);
