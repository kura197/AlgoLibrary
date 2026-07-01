#pragma once

/// 2次元セグメント木
/// 参考: http://algoogle.hadrori.jp/algorithm/2d-segment-tree.html
/// https://atcoder.jp/contests/abc228/submissions/me
/// TODO: 抽象化

struct segtree {
    ll H, W;
    vector<vector<ll>> dat;
    segtree(){}
    segtree(vector<vector<ll>> &f) {
        H = W = 1;
        while(H < (ll)f.size()) H <<= 1;
        while(W < (ll)f[0].size()) W <<= 1;
        dat.assign(2*H-1,vector<ll>(2*W-1, INF));
        init(f);
    }

    void init(vector<vector<ll>> &f) {
        for (ll i = 0; i < (ll)f.size(); i++)
            for (ll j = 0; j < (ll)f[0].size(); j++)
                dat[i+H-1][j+W-1] = f[i][j];
        for (ll i = 2*H-2; i > H-2; i--)
            for (ll j = W-2; j >= 0; j--)
                dat[i][j] = max(dat[i][2*j+1], dat[i][2*j+2]);
        for (ll i = H-2; i >= 0; i--)
            for (ll j = 0; j < 2*W-1; j++)
                dat[i][j] = max(dat[2*i+1][j], dat[2*i+2][j]);
    }

    //// return max of range( y:[li, ri), x:[lj, rj) )
    ll maximum(ll li, ll lj, ll ri, ll rj) { return maximum_h(li,lj,ri,rj,0,H,0); }

    ll maximum_h(ll li, ll lj, ll ri, ll rj, ll si, ll ti, ll k) {
        if(ri <= si or ti <= li) return -INF;
        if(li <= si and ti <= ri) return maximum_w(lj,rj,0,W,k,0);
        const ll mi = (si+ti)/2;
        return max(maximum_h(li,lj,ri,rj,si,mi,2*k+1), maximum_h(li,lj,ri,rj,mi,ti,2*k+2));
    }

    ll maximum_w(ll lj, ll rj, ll sj, ll tj, ll i, ll k) {
        if(rj <= sj or tj <= lj) return -INF;
        if(lj <= sj and tj <= rj) return dat[i][k];
        const ll mj = (sj+tj)/2;
        return max(maximum_w(lj,rj,sj,mj,i,2*k+1),maximum_w(lj,rj,mj,tj,i,2*k+2));
    }
};
