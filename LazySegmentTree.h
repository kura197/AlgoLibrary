#pragma once

//// 遅延評価
//// https://atcoder.jp/contests/abl/editorial/1204
//// https://atcoder.jp/contests/typical90/submissions/25318781
//// lazyを含めて抽象化

//// 基本的な動作はSegTreeと同じ。
//// 区間updateをする際には、update値が葉に向かって進んでいく。
//// 区間が完全に一致したノードにおいて、それ以降葉に進んでいくのを止め、lazy配列を更新する(update)。
//// あるノードを遅延評価するときに、より末端のノードに向かってlazy値を転送する(eval)

//// lazyの型をFとする。
//// composition: 新しいlazyの値の使用方法 (古いlazyに加算, 上書き, ...) 
//// mapping: lazyの値からnodeの値へのアップデート方法 (例: 区間加算後に区間minを求める --> node += lazy?)

//// class S: type of monoid.
//// S init(): return initial value which satisfies {op(s, init()) = op(init(), s) = s}.
//// S op(S, S): function which satisfies monoid property.
//// class F: type of lazy
//// S mapping(F, S): node[k] = apply(lazy[k], node[k]);
//// F composition(F, F): lazy[k] = composition(lazy[k], f);
//// F id(): return identity function which satisfies {apply(id(), f) = apply(f, id()) = f}

template<class S, S (*op)(S, S), S (*init)(), class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>
class LazySegmentTree{
    ll n;
    vector<S> node;
    vector<F> lazy;
    vector<bool> lazyFlag;

public:
    LazySegmentTree(vector<S> v){
        int size = v.size();
        n = 1;
        while(n < size) n *= 2;
        node.resize(2*n-1, init());
        lazy.resize(2*n-1, id());
        lazyFlag.resize(2*n-1, false);

        for(int i = 0; i < size; i++){
            node[n-1+i] = v[i];
        }
        for(int i = n-2; i >= 0; i--){
            node[i] = op(node[2*i+1], node[2*i+2]);
        }
    }

    // k番目のノードについて遅延評価
    void eval(int k, int l, int r){
        if(lazyFlag[k]){
            if(r - l > 1){
                lazy[2*k+1] = composition(lazy[2*k+1], lazy[k]);
                lazy[2*k+2] = composition(lazy[2*k+2], lazy[k]);
                lazyFlag[2*k+1] = lazyFlag[2*k+2] = true;
            }

            node[k] = mapping(lazy[k], node[k]);

            lazy[k] = id();
            lazyFlag[k] = false;
        }
    }

    // 区間更新
    // update val in [a, b] --> update(a, b+1)
    void update(int a, int b, F x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;

        eval(k, l, r);

        if(b <= l || r <= a) return;

        if(a <= l && r <= b){
            lazy[k] = composition(lazy[k], x);
            lazyFlag[k] = true;
            eval(k, l, r);
        }
        else{
            update(a, b, x, 2*k+1, l, (l+r)/2);
            update(a, b, x, 2*k+2, (l+r)/2, r);
            node[k] = op(node[2*k+1], node[2*k+2]);
        }
    }

    void abs_update(int idx, S val){
        idx += n-1;
        node[idx] = val;
        while(idx > 0){
            idx = (idx-1)/2;
            node[idx] = op(node[2*idx+1], node[2*idx+2]);
        }
    }

    // get val in [a, b] --> get_val(a, b+1)
    S get_val(int a, int b, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;

        if(b <= l || r <= a) return init();

        eval(k, l, r);

        if(a <= l && r <= b) return node[k];

        S vl = get_val(a, b, 2*k+1, l, (l+r)/2);
        S vr = get_val(a, b, 2*k+2, (l+r)/2, r);
        return op(vl, vr);
    }
};

struct S {
    ll x;
    ll y;
};

/// 例
S op(S lhs, S rhs){
    ll val = lhs.x * ten[rhs.y] + rhs.x;
    val %= MOD;
    return S{val, lhs.y + rhs.y};
}

S init(){
    return S{0, 0};
}

using F = long long;

S mapping(F x, S s){
    if(x == 0) return s;
    ll val = x * one[s.y];
    val %= MOD;
    return S{val, s.y};
}

F composition(F lhs, F rhs){
    return rhs;
}

F id(){
    return 0;
}

vector<S> vec(N);
LazySegmentTree<S, op, init, F, mapping, composition, id> stree(vec);



//// 典型029 (https://atcoder.jp/contests/typical90/tasks/typical90_ac)
//// 遅延評価
//// TODO: 区間overwrite

///  初期値,関数指定
template<ll init, ll (*op)(ll, ll)>
class LazySegmentTree{
    ll n;
    vector<ll> node, lazy;
    vector<bool> lazyFlag;

public:
    LazySegmentTree(vector<ll> v){
        int size = v.size();
        n = 1;
        while(n < size) n *= 2;
        node.resize(2*n-1, init);
        lazy.resize(2*n-1, init);
        lazyFlag.resize(2*n-1, false);

        for(int i = 0; i < size; i++){
            node[n-1+i] = v[i];
        }
        for(int i = n-2; i >= 0; i--){
            node[i] = op(node[2*i+1], node[2*i+2]);
        }
    }

    // k番目のノードについて遅延評価
    void eval(int k, int l, int r){
        if(lazyFlag[k]){
            if(r - l > 1){
                lazy[2*k+1] = op(lazy[2*k+1], lazy[k]);
                lazy[2*k+2] = op(lazy[2*k+2], lazy[k]);
                lazyFlag[2*k+1] = lazyFlag[2*k+2] = true;
            }

            node[k] = op(node[k], lazy[k]);
            ///TODO:
            lazy[k] = init;

            lazyFlag[k] = false;
        }
    }

    // 区間更新
    void update(int a, int b, ll x, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;

        eval(k, l, r);

        if(b <= l || r <= a) return;

        if(a <= l && r <= b){
            lazy[k] = op(lazy[k], x);
            lazyFlag[k] = true;
            eval(k, l, r);
        }
        else{
            update(a, b, x, 2*k+1, l, (l+r)/2);
            update(a, b, x, 2*k+2, (l+r)/2, r);
            node[k] = op(node[2*k+1], node[2*k+2]);
        }
    }

    void abs_update(int idx, int val){
        idx += n-1;
        node[idx] = val;
        while(idx > 0){
            idx = (idx-1)/2;
            node[idx] = op(node[2*idx+1], node[2*idx+2]);
        }
    }

    //call get_val(a, b);
    ll get_val(int a, int b, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;

        if(b <= l || r <= a) return init;

        eval(k, l, r);

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

LazySegmentTree<0, my_sum> stree(vec);
LazySegmentTree<-INF, my_max> stree(vec);


/////////////////////////

// https://github.com/atcoder/ac-library/blob/master/document_ja/lazysegtree.md
// 区間最大値取得 + 区間更新

// #include <atcoder/all>

using S = int;

S op(S a, S b) {
    return max(a, b);
}

S e() {
    return 0;
}

using F = int;

// f: 更新操作
// x: 現在の値
S mapping(F f, S x) {
    if (f == -1) return x;  // 更新なし
    return f;               // 区間代入
}

// f after g
F composition(F f, F g) {
    if (f == -1) return g;  // 新しい更新がないなら古い更新を残す
    return f;               // 新しい代入で上書き
}

F id() {
    return -1;              // 更新なし
}

lazy_segtree<S, op, e, F, mapping, composition, id> seg(W+1);
