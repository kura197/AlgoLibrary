#pragma once

#include <algorithm>
#include <vector>

using namespace std;

/// 2次元セグメント木
/// 参考: http://algoogle.hadrori.jp/algorithm/2d-segment-tree.html
/// https://atcoder.jp/contests/abc228/submissions/me
/// TODO: 抽象化

struct segtree {
    long long H, W;
    vector<vector<long long>> dat;
    segtree(){}
    segtree(vector<vector<long long>> &f) {
        H = W = 1;
        while(H < (long long)f.size()) H <<= 1;
        while(W < (long long)f[0].size()) W <<= 1;
        dat.assign(2*H-1,vector<long long>(2*W-1, INF));
        init(f);
    }

    void init(vector<vector<long long>> &f) {
        for (long long i = 0; i < (long long)f.size(); i++)
            for (long long j = 0; j < (long long)f[0].size(); j++)
                dat[i+H-1][j+W-1] = f[i][j];
        for (long long i = 2*H-2; i > H-2; i--)
            for (long long j = W-2; j >= 0; j--)
                dat[i][j] = max(dat[i][2*j+1], dat[i][2*j+2]);
        for (long long i = H-2; i >= 0; i--)
            for (long long j = 0; j < 2*W-1; j++)
                dat[i][j] = max(dat[2*i+1][j], dat[2*i+2][j]);
    }

    //// return max of range( y:[li, ri), x:[lj, rj) )
    long long maximum(long long li, long long lj, long long ri, long long rj) { return maximum_h(li,lj,ri,rj,0,H,0); }

    long long maximum_h(long long li, long long lj, long long ri, long long rj, long long si, long long ti, long long k) {
        if(ri <= si or ti <= li) return -INF;
        if(li <= si and ti <= ri) return maximum_w(lj,rj,0,W,k,0);
        const long long mi = (si+ti)/2;
        return max(maximum_h(li,lj,ri,rj,si,mi,2*k+1), maximum_h(li,lj,ri,rj,mi,ti,2*k+2));
    }

    long long maximum_w(long long lj, long long rj, long long sj, long long tj, long long i, long long k) {
        if(rj <= sj or tj <= lj) return -INF;
        if(lj <= sj and tj <= rj) return dat[i][k];
        const long long mj = (sj+tj)/2;
        return max(maximum_w(lj,rj,sj,mj,i,2*k+1),maximum_w(lj,rj,mj,tj,i,2*k+2));
    }
};
