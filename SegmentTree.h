#pragma once

#include <vector>

const long long INF = 10000000;
class SegmentTree{
    long long n;
    vector<long long> node;

public:
    SegmentTree(vector<long long> v){ 
        long long size = v.size();
        n = 1;
        while(n < size) n *= 2;
        node.resize(2*n-1, INF);

        for(long long i = 0; i < size; i++){
            node[n-1+i] = v[i];
        }
        for(long long i = n-2; i >= 0; i--){
            node[i] = min(node[2*i+1], node[2*i+2]);
        }
    }

    void update(long long idx, long long val){
        idx += n-1;
        node[idx] = val;
        while(idx > 0){
            idx = (idx-1)/2;
            node[idx] = min(node[2*idx+1], node[2*idx+2]);
        }
    }

    //return min value within [a,b)
    //call get_min(a, b);
    // 0-index ??
    long long get_min(long long a, long long b, long long k=0, long long l=0, long long r=-1){
        if(r < 0) r = n;

        if(b <= l || r <= a) return INF;

        if(a <= l && r <= b) return node[k];

        long long vl = get_min(a, b, 2*k+1, l, (l+r)/2);
        long long vr = get_min(a, b, 2*k+2, (l+r)/2, r);
        return min(vl, vr);
    }
};

// ABC 179F
// 遅延評価
class LazySegmentTree{
    int n;
    vector<long long> node, lazy;
    vector<bool> lazyFlag;

public:
    LazySegmentTree(vector<long long> v){
        int size = v.size();
        n = 1;
        while(n < size) n *= 2;
        node.resize(2*n-1);
        lazy.resize(2*n-1, INF);
        lazyFlag.resize(2*n-1, false);

        for(int i = 0; i < size; i++){
            node[n-1+i] = v[i];
        }
        for(int i = n-2; i >= 0; i--){
            node[i] = min(node[2*i+1], node[2*i+2]);
        }
    }

    // k番目のノードについて遅延評価
    void eval(int k, int l, int r){
        if(lazyFlag[k]){
            if(r - l > 1){
                chmin(lazy[2*k+1], lazy[k]);
                chmin(lazy[2*k+2], lazy[k]);
                lazyFlag[2*k+1] = lazyFlag[2*k+2] = true;
            }

            node[k] = min(node[k], lazy[k]);

            lazyFlag[k] = false;
        }
    }

    // 区間更新
    void update(int a, int b, long long x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;

        eval(k, l, r);

        if(b <= l || r <= a) return;

        if(a <= l && r <= b){
            lazy[k] = min(lazy[k], x);
            lazyFlag[k] = true;
            eval(k, l, r);
        }
        else{
            update(a, b, x, 2*k+1, l, (l+r)/2);
            update(a, b, x, 2*k+2, (l+r)/2, r);
            node[k] = min(node[2*k+1], node[2*k+2]);
        }
    }

    void abs_update(int idx, int val){
        idx += n-1;
        node[idx] = val;
        while(idx > 0){
            idx = (idx-1)/2;
            node[idx] = min(node[2*idx+1], node[2*idx+2]);
        }
    }

    //return min value within [a,b)
    //call get_min(a, b);
    long long get_min(int a, int b, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;

        if(b <= l || r <= a) return INF;

        eval(k, l, r);

        if(a <= l && r <= b) return node[k];

        int vl = get_min(a, b, 2*k+1, l, (l+r)/2);
        int vr = get_min(a, b, 2*k+2, (l+r)/2, r);
        return min(vl, vr);
    }
};
