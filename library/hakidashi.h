#pragma once

#include <algorithm>
#include <vector>

using namespace std;

/// WIP
/// 掃き出し法 (GF(2))
/// https://atcoder.jp/contests/typical90/tasks/typical90_be

/// E.g. mat = [[1, 1, 0, 1, 0, ...],
///             [1, 0, 1, 1, 1, ...],
///             ...,
///             [0, 1, 1, 1, 1, ...]]
/// return rank
int hakidashi(vector<vector<int>>& mat){
    int N = mat.size();
    int M = mat[0].size();

    int row = 0;
    for(int j = 0; j < M && row < N; j++){
        bool found = false;

        /// find 1
        for (int i = row; i < N; i++) {
            if(mat[i][j]){
                swap(mat[row], mat[i]);
                found = true;
                break;
            }
        }

        if(!found) continue;

        /// update row+k (k > 0)
        for (int i = row + 1; i < N; i++) {
            if(mat[i][j]){
                for (int k = 0; k < M; k++) {
                    mat[i][k] ^= mat[row][k];
                }
            }
        }

        row++;
    }

    return row;
}


//// 参考: https://qiita.com/Sooh31/items/97e2d9724fe47ed238b3
//// XORの基底をbaseに格納
vector<int> get_base(vector<int>& A){
    vector<int> base;
    for(auto v : A){
        for(auto e : base){
            v = min(v, v ^ e);
        }
        if(v > 0) base.push_back(v);
    }    
    return base;
}


//////////////////////////////////
/// 連立一次方程式 O(RC^2)
/// 参考: https://drken1215.hatenablog.com/entry/2019/03/20/202800
/// submit: https://codeforces.com/contest/1155/submission/144295799

// 逆元計算
long long modinv(long long a, long long mod) {
    long long b = mod, u = 1, v = 0;
    while (b) {
        long long t = a/b;
        a -= t*b; swap(a, b);
        u -= t*v; swap(u, v);
    }
    u %= mod;
    if (u < 0) u += mod;
    return u;
}

template<int MOD> struct Matrix {
    vector<vector<long long> > val;
    Matrix(int n, int m, long long x = 0) : val(n, vector<long long>(m, x)) {}
    void init(int n, int m, long long x = 0) {val.assign(n, vector<long long>(m, x));}
    size_t size() const {return val.size();}
    inline vector<long long>& operator [] (int i) {return val[i];}
};

template<int MOD> int GaussJordan(Matrix<MOD> &A, bool is_extended = false) {
    int m = A.size(), n = A[0].size();
    for (int row = 0; row < m; ++row)
        for (int col = 0; col < n; ++col)
            A[row][col] = (A[row][col] % MOD + MOD) % MOD;
    
    int rank = 0;
    for (int col = 0; col < n; ++col) {
        if (is_extended && col == n-1) break;
        int pivot = -1;
        for (int row = rank; row < m; ++row) {
            if (A[row][col] != 0) {
                pivot = row;
                break;
            }
        }
        if (pivot == -1) continue;
        swap(A[pivot], A[rank]);
        auto inv = modinv(A[rank][col], MOD);
        for (int col2 = 0; col2 < n; ++col2)
            A[rank][col2] = A[rank][col2] * inv % MOD;
        for (int row = 0; row < m; ++row) {
            if (row != rank && A[row][col]) {
                auto fac = A[row][col];
                for (int col2 = 0; col2 < n; ++col2) {
                    A[row][col2] -= A[rank][col2] * fac % MOD;
                    if (A[row][col2] < 0) A[row][col2] += MOD;
                }
            }
        }
        ++rank;
    }
    return rank;
}

template<int MOD> int linear_equation(Matrix<MOD> A, vector<long long> b, vector<long long> &res) {
    int m = A.size(), n = A[0].size();
    Matrix<MOD> M(m, n + 1);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) M[i][j] = A[i][j];
        M[i][n] = b[i];
    }
    int rank = GaussJordan(M, true);

    // check if it has no solution
    for (int row = rank; row < m; ++row) if (M[row][n]) return -1;

    // answer
    res.assign(n, 0);
    for (int i = 0; i < rank; ++i) res[i] = M[i][n];
    return rank;
}
