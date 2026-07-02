#pragma once

#include <vector>

////  初期値,関数指定
template<long long init, long long (*op)(long long, long long)>
class SegmentTree{
    long long n;
    vector<long long> node;

public:
    SegmentTree(vector<long long> v){ 
        long long size = v.size();
        n = 1;
        while(n < size) n *= 2;
        node.resize(2*n-1, init);

        for(long long i = 0; i < size; i++){
            node[n-1+i] = v[i];
        }
        for(long long i = n-2; i >= 0; i--){
            node[i] = op(node[2*i+1], node[2*i+2]);
        }
    }

    void overwrite(long long idx, long long val){
        idx += n-1;
        node[idx] = val;
        while(idx > 0){
            idx = (idx-1)/2;
            node[idx] = op(node[2*idx+1], node[2*idx+2]);
        }
    }

    void update(long long idx, long long val){
        idx += n-1;
        node[idx] = op(node[idx], val);
        while(idx > 0){
            idx = (idx-1)/2;
            node[idx] = op(node[2*idx+1], node[2*idx+2]);
        }
    }

    //return {max, min, ...} value within [a,b)
    //call get_val(a, b);
    long long get_val(long long a, long long b, long long k=0, long long l=0, long long r=-1){
        if(r < 0) r = n;

        if(b <= l || r <= a) return init;

        if(a <= l && r <= b) return node[k];

        long long vl = get_val(a, b, 2*k+1, l, (l+r)/2);
        long long vr = get_val(a, b, 2*k+2, (l+r)/2, r);
        return op(vl, vr);
    }
};

/// 例
long long my_max(long long x, long long y){
    return max(x, y);
}

long long my_sum(long long x, long long y){
    return x + y;
}

SegmentTree<0, my_sum> stree(vec);
SegmentTree<-INF, my_max> stree(vec);



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/// ACL (-I/home/kura/Documents/atcoder/ac-library)

#include <atcoder/all>

using namespace atcoder;


using S = pair<double,double>;

S op(S a, S b){
    return S{a.first + b.first, a.second + b.second};
}

S e(){
    return S{0, 0};
}

int target_right;
int target_left;

bool f_right(S x){
    return x < target_right;
}

bool f_left(S x){
    return x < target_left;
}

segtree<S, op, e> seg(M+N+1);

seg.set(a, S{0, 1.0});  //// overwrite
auto val = seg.prod(left, right);   /// [left, right)
auto x = seg.get(1);

auto tmp = seg.max_right<f_right>(table[x]);
long long ans = (tmp == cnt-1) ? -1 : rev_table[tmp];

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/// 抽象化

template<class S, S (*op)(S, S), S (*init)()>
class SegmentTree{
    int n;
    vector<S> node;
 
public:
    SegmentTree(vector<S> v){ 
        int size = v.size();
        n = 1;
        while(n < size) n *= 2;
        node.resize(2*n-1, init());
 
        for(int i = 0; i < size; i++){
            node[n-1+i] = v[i];
        }
        for(int i = n-2; i >= 0; i--){
            node[i] = op(node[2*i+1], node[2*i+2]);
        }
    }
 
    void overwrite(int idx, S val){
        idx += n-1;
        node[idx] = val;
        while(idx > 0){
            idx = (idx-1)/2;
            node[idx] = op(node[2*idx+1], node[2*idx+2]);
        }
    }
 
    void update(int idx, S val){
        idx += n-1;
        node[idx] = op(node[idx], val);
        while(idx > 0){
            idx = (idx-1)/2;
            node[idx] = op(node[2*idx+1], node[2*idx+2]);
        }
    }
 
    //return {max, min, ...} value within [a,b)
    //call get_val(a, b);
    S get_val(int a, int b, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;
 
        if(b <= l || r <= a) return init();
 
        if(a <= l && r <= b) return node[k];
 
        auto vl = get_val(a, b, 2*k+1, l, (l+r)/2);
        auto vr = get_val(a, b, 2*k+2, (l+r)/2, r);
        return op(vl, vr);
    }
};
 
const int K = 9;
 
struct S{
    long long val;
    array<long long, K> mi;
};
 
S init(){
    S z;
    z.val = INF;
    REP(i,K) z.mi[i] = INF;
    return z;
}
 
/// 例
S my_min(S x, S y){
    S z = init();
    z.val = min(x.val, y.val);
    REP(k,K){
        chmin(z.val, x.mi[k] + y.mi[k]);
        chmin(z.mi[k], min(x.mi[k], y.mi[k]));
    }
    return z;
}

SegmentTree<S, my_min, init> stree(X);
