#include <vector>

////  初期値,関数指定
template<ll init, ll (*op)(ll, ll)>
class SegmentTree{
    ll n;
    vector<ll> node;

public:
    SegmentTree(vector<ll> v){ 
        ll size = v.size();
        n = 1;
        while(n < size) n *= 2;
        node.resize(2*n-1, init);

        for(ll i = 0; i < size; i++){
            node[n-1+i] = v[i];
        }
        for(ll i = n-2; i >= 0; i--){
            node[i] = op(node[2*i+1], node[2*i+2]);
        }
    }

    void overwrite(ll idx, ll val){
        idx += n-1;
        node[idx] = val;
        while(idx > 0){
            idx = (idx-1)/2;
            node[idx] = op(node[2*idx+1], node[2*idx+2]);
        }
    }

    void update(ll idx, ll val){
        idx += n-1;
        node[idx] = op(node[idx], val);
        while(idx > 0){
            idx = (idx-1)/2;
            node[idx] = op(node[2*idx+1], node[2*idx+2]);
        }
    }

    //return {max, min, ...} value within [a,b)
    //call get_val(a, b);
    ll get_val(ll a, ll b, ll k=0, ll l=0, ll r=-1){
        if(r < 0) r = n;

        if(b <= l || r <= a) return init;

        if(a <= l && r <= b) return node[k];

        ll vl = get_val(a, b, 2*k+1, l, (l+r)/2);
        ll vr = get_val(a, b, 2*k+2, (l+r)/2, r);
        return op(vl, vr);
    }
};

/// 例
ll my_max(ll x, ll y){
    return max(x, y);
}

ll my_sum(ll x, ll y){
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
ll ans = (tmp == cnt-1) ? -1 : rev_table[tmp];

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
    ll val;
    array<ll, K> mi;
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
