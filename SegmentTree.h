#include <vector>

const ll INF = 10000000;
class SegmentTree{
    ll n;
    vector<ll> node;

public:
    SegmentTree(vector<ll> v){ 
        ll size = v.size();
        n = 1;
        while(n < size) n *= 2;
        node.resize(2*n-1, INF);

        for(ll i = 0; i < size; i++){
            node[n-1+i] = v[i];
        }
        for(ll i = n-2; i >= 0; i--){
            node[i] = min(node[2*i+1], node[2*i+2]);
        }
    }

    void update(ll idx, ll val){
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
    ll get_min(ll a, ll b, ll k=0, ll l=0, ll r=-1){
        if(r < 0) r = n;

        if(b <= l || r <= a) return INF;

        if(a <= l && r <= b) return node[k];

        ll vl = get_min(a, b, 2*k+1, l, (l+r)/2);
        ll vr = get_min(a, b, 2*k+2, (l+r)/2, r);
        return min(vl, vr);
    }
};

// ABC 179F
// 遅延評価
class LazySegmentTree{
    int n;
    vector<ll> node, lazy;
    vector<bool> lazyFlag;

public:
    LazySegmentTree(vector<ll> v){
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
    void update(int a, int b, ll x, int k=0, int l=0, int r=-1){
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
    ll get_min(int a, int b, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;

        if(b <= l || r <= a) return INF;

        eval(k, l, r);

        if(a <= l && r <= b) return node[k];

        int vl = get_min(a, b, 2*k+1, l, (l+r)/2);
        int vr = get_min(a, b, 2*k+2, (l+r)/2, r);
        return min(vl, vr);
    }
};
