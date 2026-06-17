
//Binary Indexed tree


//ARC 033 C
/// BIT上の２分探索. (中央値の探索)
/// 参考: https://atcoder.jp/contests/abc218/submissions/28217585

template<class T> class BIT {
public:
    vector<T> data;
    int N;

    BIT(){}
    BIT(int N) : N(N) { data.assign(N,0); }

    // sum [0,i)
    T sum(int i){
        int ret = 0;
        for(--i; i>=0; i=(i&(i+1))-1) ret += data[i];
        return ret;
    }

    // sum [i,j)
    T sum(int i, int j) { return sum(j) - sum(i); }

    // add x to i
    void add(int i, T x) { for(; i < N; i|=i+1) data[i] += x; }

    //// binary search
    //// return i, s.t. sum [0, i) >= w ??
    T lowerBound(T w){
        if(w <= 0) return 0;
        T x = 0;
        T k = 1;
        while(k * 2 <= N) k *= 2;
        for(; k > 0; k /= 2){
            if(x+k-1 <= N && data[x+k-1] < w){
                w -= data[x+k-1];
                x += k;
            }
        }
        return x + 1;
    }
};


/////////////////////////////////////////

//Binary Indexed tree
//add w to bit[a]
//sum of [bit[1], bit[2], ..., bit[a]]
const int MAX_N = 1000100;
int N;
int bit[MAX_N];

// 1-indexed
void add(int a, int w){
    for(int x = a; x <= MAX_N; x += x & -x) bit[x] += w;
}

// 1-indexed
int sum(int a){
    int ret = 0;
    for(int x = a; x > 0; x -= x & -x) ret += bit[x];
    return ret;
}

// 0-indexed
void add(int a, int w){
    for(int x = a; x < MAX_N; x |= x + 1) bit[x] += w;
}

// 0-indexed
int sum(int a){
    int ret = 0;
    for(int x = a-1; x >= 0; x = (x & (x+1)) - 1) ret += bit[x];
    return ret;
}

////////////////////////

//ARC 075E

//1-indexed
struct BIT{
    int N;
    vector<int> data;

    BIT(int n){
        N = n;
        data.resize(N+1, 0);
    }

    // 1-indexed
    void add(int a, int w){
        for(int x = a; x <= N; x += x & -x)
            data[x] += w;
    }

    // sum of d[1] + d[1] + ... + d[a];
    int sum(int a){
        int ret = 0;
        for(int x = a; x > 0; x -= x & -x)
            ret += data[x];
        return ret;
    }

};

//0-indexed
struct BIT{
    int N;
    vector<int> data;

    BIT(int n){
        N = n;
        data.resize(N, 0);
    }

    // 0-indexed
    void add(int a, int w){
        for(int x = a; x < N; x |= x + 1)
            data[x] += w;
    }

    // sum of d[0] + d[1] + ... + d[a-1];
    int sum(int a){
        int ret = 0;
        for(int x = a-1; x >= 0; x = (x & (x+1)) - 1)
            ret += data[x];
        return ret;
    }
};
