// 文字列検索など
// ABC141E

//////////////////
/// ACL (https://atcoder.github.io/ac-library/document_ja/string.html)

#include <atcoder/all>

int op(int a, int b) {
    return min(a, b);
}

int e() {
    return (int)(1e9);
}

 vector<int> sa = suffix_array(S);
 vector<int> lcp = lcp_array(S, sa);
 segtree<int, op, e> seg(lcp);

/////////////////


/// 要確認

class SuffixArray{
    // 任意のsa間で一致数をカウント
    class SegmentTree{
        const int INF = 10000000;
        int n;
        vector<int> node;

    public:
        SegmentTree(){}

        SegmentTree(const vector<int>& v){
            init(v);
        }

        void init(const vector<int>& v){
            int size = v.size();
            n = 1;
            while(n < size) n *= 2;
            node.resize(2*n-1, INF);

            for(int i = 0; i < size; i++){
                node[n-1+i] = v[i];
            }
            for(int i = n-2; i >= 0; i--){
                node[i] = min(node[2*i+1], node[2*i+2]);
            }
        }

        void update(int idx, int val){
            idx += n-1;
            node[idx] = val;
            while(idx > 0){
                idx = (idx-1)/2;
                node[idx] = min(node[2*idx+1], node[2*idx+2]);
            }
        }

        //return min value within [a,b)
        //call get_min(a, b);
        int get_min(int a, int b, int k=0, int l=0, int r=-1){
            if(r < 0) r = n;

            if(b <= l || r <= a) return INF;

            if(a <= l && r <= b) return node[k];

            int vl = get_min(a, b, 2*k+1, l, (l+r)/2);
            int vr = get_min(a, b, 2*k+2, (l+r)/2, r);
            return min(vl, vr);
        }
    };

    public:
    ll n, k;
    vector<int> order;
    vector<int> sa;
    vector<int> lcp;
    //// index of string --> index of sa
    //// sa[point[i]] = i
    vector<int> point;
    SegmentTree stree;
    string S;

    SuffixArray(const string& _S): S(_S){
        construct_sa();
        construct_lcp();
        stree.init(lcp);
        point.resize(n+1);
        for(int i = 0; i <= n; i++) point[sa[i]] = i;
    }

    //文字列Sの接尾辞配列を構築 (O(n*logn*logn))
    void construct_sa(){
        n = S.length();
        sa.resize(n+1);
        order.resize(n+1);
        vector<ll> tmp(n+1);
        
        //最初の1文字. ランクを文字コードで表現
        for(int i = 0; i <= n; i++){
            sa[i] = i;
            order[i] = (i < n) ? S[i] : -1;
        }

        //k文字についてソートされているところから、2k文字でソートする
        for(k = 1; k <= n; k *= 2){
            // (order[i], order[i + k])と(order[j], order[j + k])を比較.
            auto compare_sa = [&](int i, int j){
                if(order[i] != order[j]) return order[i] < order[j];
                else {
                    int ri = (i + k <= n) ? order[i + k] : -1;
                    int rj = (j + k <= n) ? order[j + k] : -1;
                    return ri < rj;
                }
            };

            sort(sa.begin(), sa.end(), compare_sa);

            //tmpに次のランクを格納. 辞書順にランク計算.
            tmp[sa[0]] = 0;
            for(int i = 1; i <= n; i++){
                tmp[sa[i]] = tmp[sa[i-1]] + (compare_sa(sa[i-1], sa[i]) ? 1 : 0);
            }
            for(int i = 0; i <= n; i++){
                order[i] = tmp[i];
            }
        }
    }

    // 文字列Sとその接尾辞配列saを受け取り、高さ配列lcpを返す. (O(n))
    void construct_lcp(){
        n = S.length();
        lcp.resize(n);
        order.resize(n+1);

        for(int i = 0; i <= n; i++)
            order[sa[i]] = i;

        int h = 0;
        lcp[0] = 0;
        for(int i = 0; i < n; i++){
            // 文字列中での位置iの接尾辞と、接尾辞配列中でその一つ前の設備時のLCPを求める.
            int j = sa[order[i] - 1];

            // hを先頭の分1減らし、後ろが一致しているだけ増やす.
            if(h > 0) h--;
            for(; j + h < n && i + h < n; h++){
                if(S[j + h] != S[i + h]) break;
            }

            lcp[order[i] - 1] = h;
        }
    }

    ///文字列TがSに含まれるかどうか
    bool contain(string T){
        int a = 0, b = S.length();
        while(b - a > 1){
            int c = (a + b) / 2;
            if(S.compare(sa[c], T.length(), T) < 0) a = c;
            else b = c;
        }
        return S.compare(sa[b], T.length(), T) == 0;
    }

    //// (i, j) : sa配列のインデックス
    //// 文字列S のsa[i]番目から始まる接尾辞とsa[j]番目から始まる接尾辞の共通文字数を返す
    int get_match(int i, int j){
        if(i == j) return n - sa[i];
        return stree.get_min(i, j);
    }

    //// (i, j) : sa配列のインデックス
    //// 文字列S のsa[i]番目から始まる接尾辞とsa[j]番目から始まる接尾辞との文字列間の長さを返す
    int get_dist(int i, int j){
        int x = sa[i], y = sa[j];
        if(x > y) swap(x, y);
        return y - x;
    }
};



/////////////////////////////////////////
/////////////////////////////////////////

ll n, k;
vector<int> order;
vector<int> sa;
vector<int> lcp;

// (order[i], order[i + k])と(order[j], order[j + k])を比較.
bool compare_sa(int i, int j){
    if(order[i] != order[j]) return order[i] < order[j];
    else {
        int ri = (i + k <= n) ? order[i + k] : -1;
        int rj = (j + k <= n) ? order[j + k] : -1;
        return ri < rj;
    }
}

//文字列Sの接尾辞配列を構築 (O(n*logn*logn))
void construct_sa(string S){
    n = S.length();
    sa.resize(n+1);
    order.resize(n+1);
    vector<ll> tmp(n+1);
    
    //最初の1文字. ランクを文字コードで表現
    for(int i = 0; i <= n; i++){
        sa[i] = i;
        order[i] = (i < n) ? S[i] : -1;
    }

    //k文字についてソートされているところから、2k文字でソートする
    for(k = 1; k <= n; k *= 2){
        sort(sa.begin(), sa.end(), compare_sa);

        //tmpに次のランクを格納. 辞書順にランク計算.
        tmp[sa[0]] = 0;
        for(int i = 1; i <= n; i++){
            tmp[sa[i]] = tmp[sa[i-1]] + (compare_sa(sa[i-1], sa[i]) ? 1 : 0);
        }
        for(int i = 0; i <= n; i++){
            order[i] = tmp[i];
        }
    }
}

bool contain(string S, string T){
    int a = 0, b = S.length();
    while(b - a > 1){
        int c = (a + b) / 2;
        if(S.compare(sa[c], T.length(), T) < 0) a = c;
        else b = c;
    }
    return S.compare(sa[b], T.length(), T) == 0;
}

// 文字列Sとその接尾辞配列saを受け取り、高さ配列lcpを返す. (O(n))
void construct_lcp(string S){
    n = S.length();
    lcp.resize(n);
    order.resize(n+1);

    for(int i = 0; i <= n; i++)
        order[sa[i]] = i;

    int h = 0;
    lcp[0] = 0;
    for(int i = 0; i < n; i++){
        // 文字列中での位置iの接尾辞と、接尾辞配列中でその一つ前の設備時のLCPを求める.
        int j = sa[order[i] - 1];

        // hを先頭の分1減らし、後ろが一致しているだけ増やす.
        if(h > 0) h--;
        for(; j + h < n && i + h < n; h++){
            if(S[j + h] != S[i + h]) break;
        }

        lcp[order[i] - 1] = h;
    }
}

////////////////////////////////
// 任意のsa間で一致数をカウント
const int INF = 10000000;
class SegmentTree{
    int n;
    vector<int> node;

public:
    SegmentTree(vector<int> v){
        int size = v.size();
        n = 1;
        while(n < size) n *= 2;
        node.resize(2*n-1, INF);

        for(int i = 0; i < size; i++){
            node[n-1+i] = v[i];
        }
        for(int i = n-2; i >= 0; i--){
            node[i] = min(node[2*i+1], node[2*i+2]);
        }
    }

    void update(int idx, int val){
        idx += n-1;
        node[idx] = val;
        while(idx > 0){
            idx = (idx-1)/2;
            node[idx] = min(node[2*idx+1], node[2*idx+2]);
        }
    }

    //return min value within [a,b)
    //call get_min(a, b);
    int get_min(int a, int b, int k=0, int l=0, int r=-1){
        if(r < 0) r = n;

        if(b <= l || r <= a) return INF;

        if(a <= l && r <= b) return node[k];

        int vl = get_min(a, b, 2*k+1, l, (l+r)/2);
        int vr = get_min(a, b, 2*k+2, (l+r)/2, r);
        return min(vl, vr);
    }
};

////////////////////////////////
//

// in main function
construct_sa(S);
construct_lcp(S);
SegmentTree stree(lcp);

// (i, j) : sa配列のインデックス
// (x, y) : 文字列S上での先頭インデックス
REP(i,N){
    REPi(j,i+1,N+1){
        int l = stree.get_min(i, j);
        int x, y;
        x = sa[i], y = sa[j];
        if(x > y) swap(x, y);

        // 文字列間の長さ
        int diff = y - x;
    }
}
