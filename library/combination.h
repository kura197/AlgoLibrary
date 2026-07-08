#pragma once

#include <vector>
#include <atcoder/all>

// MODありの場合 (ACL)
// 構築 O(size), comb/perm O(1)
template<int MOD>
struct Combination {
    using mint = atcoder::static_modint<MOD>;

    std::vector<mint> fact;
    std::vector<mint> frev;

    // 階乗と逆階乗を前計算する O(size)
    Combination(int size) : fact(size + 1, 1), frev(size + 1, 1) {
        for(int i = 1; i <= size; i++){
            fact[i] = fact[i - 1] * i;
        }
        frev[size] = fact[size].inv();
        for(int i = size; i >= 1; i--){
            frev[i - 1] = frev[i] * i;
        }
    }

    // nCk mod MOD を返す O(1)
    long long comb(int n, int k) const {
        if(n < 0 || k < 0 || n < k) return 0;
        if(n == 0 || k == 0) return 1;
        return (fact[n] * frev[k] * frev[n - k]).val();
    }

    // nPk mod MOD を返す O(1)
    long long perm(int n, int k) const {
        if(n < 0 || k < 0 || n < k) return 0;
        if(n == 0 || k == 0) return 1;
        return (fact[n] * frev[n - k]).val();
    }
};

// 逐次計算
// 構築 O(size^2), comb O(1)
struct CombinationTable {
    std::vector<std::vector<long long>> table;

    // Pascalの三角形を前計算する O(size^2)
    CombinationTable(int size) : table(size + 1, std::vector<long long>(size + 1, 0)) {
        for(int i = 0; i <= size; i++){
            for(int j = 0; j <= i; j++){
                if(j == 0 || j == i) table[i][j] = 1;
                else table[i][j] = table[i - 1][j - 1] + table[i - 1][j];
            }
        }
    }

    // nCk を返す O(1)
    long long comb(int n, int k) const {
        if(n < 0 || k < 0 || n < k) return 0;
        return table[n][k];
    }
};

// 1回だけ nCk mod mod を計算する
// mod は素数で、1 <= k < mod を満たす場合に使う
// O(k)
long long get_single_comb_mod(long long n, long long k, int mod){
    if(n < 0 || k < 0 || n < k) return 0;
    if(n == 0 || k == 0) return 1;

    using dynamic_mint = atcoder::dynamic_modint<0>;
    dynamic_mint::set_mod(mod);

    dynamic_mint num = 1;
    dynamic_mint den = 1;
    for(long long i = 0; i < k; i++){
        num *= n - i;
        den *= k - i;
    }

    return (num / den).val();
}
