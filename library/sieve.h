#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/sieve.h

#include <cassert>
#include <map>
#include <vector>

// 線形篩
// 構築 O(n), 素因数分解 O(log n), 約数列挙 O(約数個数)
struct Sieve {
    int built_n = -1;
    // primes[i] := i番目の素数
    std::vector<int> primes;
    // is_prime[x] := xが素数かどうか
    std::vector<bool> is_prime;
    // min_prime[x] := xを割り切る最小の素数
    std::vector<int> min_prime;
    // omega[x] := xの異なる素因数の個数
    std::vector<int> omega;
    // mobius[x] := xのメビウス関数値
    // mobius[1] = 1
    // xが同じ素因数を2回以上含むなら0
    // xが相異なるk個の素因数の積なら(-1)^k
    std::vector<int> mobius;

    Sieve() = default;
    Sieve(int n) { build(n); }

    // n以下について素数情報を前計算する O(n)
    void build(int n) {
        built_n = n;
        primes.clear();
        is_prime.assign(n + 1, false);
        min_prime.assign(n + 1, -1);
        omega.assign(n + 1, 0);
        mobius.assign(n + 1, 1);

        for (int i = 2; i <= n; i++) {
            if (min_prime[i] == -1) {
                primes.push_back(i);
                is_prime[i] = true;
                min_prime[i] = i;
                omega[i] = 1;
                mobius[i] = -1;
            }

            for (int prime : primes) {
                long long composite = (long long)i * prime;
                if (composite > n) break;

                min_prime[composite] = prime;
                if (i % prime == 0) {
                    omega[composite] = omega[i];
                    mobius[composite] = 0;
                    break;
                }
                omega[composite] = omega[i] + 1;
                mobius[composite] = -mobius[i];
            }
        }
    }

    // 素因数分解する O(log n)
    std::map<int, int> prime_factor(int n) const {
        std::map<int, int> res;
        while (n > 1) {
            res[min_prime[n]]++;
            n /= min_prime[n];
        }
        return res;
    }

    // 区間 [left, right] の各整数をまとめて素因数分解する
    // sqrt(right) <= built_n を仮定する
    // 計算量: O((right - left + 1) log log right + 区間内で実際に割る回数)
    std::vector<std::map<long long, int>> prime_factors(long long left, long long right) const {
        assert(1 <= left && left <= right);
        assert(0 <= built_n);
        assert((long long)built_n * built_n >= right);

        int length = (int)(right - left + 1);
        std::vector<long long> values(length);
        std::vector<std::map<long long, int>> factors(length);
        for (int i = 0; i < length; i++) {
            values[i] = left + i;
        }

        for (int p : primes) {
            long long prime = p;
            if (prime * prime > right) break;

            long long start = (left + prime - 1) / prime * prime;
            for (long long x = start; x <= right; x += prime) {
                int index = (int)(x - left);
                while (values[index] % prime == 0) {
                    factors[index][prime]++;
                    values[index] /= prime;
                }
            }
        }

        for (int i = 0; i < length; i++) {
            if (values[i] > 1) {
                factors[i][values[i]]++;
            }
        }

        return factors;
    }

    // 約数を列挙する O(約数個数)
    std::vector<int> divisor(int n) const {
        auto factors = prime_factor(n);
        std::vector<int> res{1};
        for (const auto& [prime, count] : factors) {
            int size = (int)res.size();
            int mul = 1;
            for (int i = 1; i <= count; i++) {
                mul *= prime;
                for (int j = 0; j < size; j++) {
                    res.push_back(res[j] * mul);
                }
            }
        }
        return res;
    }
};

// primes と is_prime だけを保持する省メモリな線形篩
// 構築 O(n), 素因数分解 O(sqrt(n) / log(n)), 約数列挙 O(sqrt(n) / log(n) + 約数個数)
struct FastSieve {
    // primes[i] := i番目の素数
    std::vector<int> primes;
    // is_prime[x] := xが素数かどうか
    std::vector<bool> is_prime;

    FastSieve() = default;
    FastSieve(int n) { build(n); }

    // n以下について素数情報を前計算する O(n)
    void build(int n) {
        primes.clear();
        is_prime.assign(n + 1, true);
        if (n >= 0) is_prime[0] = false;
        if (n >= 1) is_prime[1] = false;

        for (int i = 2; i <= n; i++) {
            if (is_prime[i]) primes.push_back(i);

            for (int prime : primes) {
                long long composite = (long long)i * prime;
                if (composite > n) break;

                is_prime[composite] = false;
                if (i % prime == 0) break;
            }
        }
    }

    // 素因数分解する
    // sqrt(n) <= 構築時の上限を仮定する
    // 計算量: O(sqrt(n) / log(n))
    std::map<int, int> prime_factor(int n) const {
        assert(1 <= n);
        int built_n = (int)is_prime.size() - 1;
        assert(n == 1 || (long long)built_n * built_n >= n);

        std::map<int, int> res;
        for (int prime : primes) {
            if ((long long)prime * prime > n) break;
            while (n % prime == 0) {
                res[prime]++;
                n /= prime;
            }
        }
        if (n > 1) res[n]++;
        return res;
    }

    // 区間 [left, right] の各整数をまとめて素因数分解する
    // sqrt(right) <= 構築時の上限を仮定する
    // 計算量: O((right - left + 1) log log right + 区間内で実際に割る回数)
    std::vector<std::map<long long, int>> prime_factors(long long left, long long right) const {
        assert(1 <= left && left <= right);
        int built_n = (int)is_prime.size() - 1;
        assert((long long)built_n * built_n >= right);

        int length = (int)(right - left + 1);
        std::vector<long long> values(length);
        std::vector<std::map<long long, int>> factors(length);
        for (int i = 0; i < length; i++) {
            values[i] = left + i;
        }

        for (int p : primes) {
            long long prime = p;
            if (prime * prime > right) break;

            long long start = (left + prime - 1) / prime * prime;
            for (long long x = start; x <= right; x += prime) {
                int index = (int)(x - left);
                while (values[index] % prime == 0) {
                    factors[index][prime]++;
                    values[index] /= prime;
                }
            }
        }

        for (int i = 0; i < length; i++) {
            if (values[i] > 1) {
                factors[i][values[i]]++;
            }
        }
        return factors;
    }

    // 約数を列挙する
    // sqrt(n) <= 構築時の上限を仮定する
    // 計算量: O(sqrt(n) / log(n) + 約数個数)
    std::vector<int> divisor(int n) const {
        auto factors = prime_factor(n);
        std::vector<int> res{1};
        for (const auto& [prime, count] : factors) {
            int size = (int)res.size();
            int mul = 1;
            for (int i = 1; i <= count; i++) {
                mul *= prime;
                for (int j = 0; j < size; j++) {
                    res.push_back(res[j] * mul);
                }
            }
        }
        return res;
    }
};

// 約数系高速ゼータ変換を行う O(N log log N)
// result[k] = sum_{k | i} f[i]
// f[0] は使わない
// primes は 2 以上 result.size() - 1 以下の素数列
// 例えば Sieve sieve((int)f.size() - 1); とすると sieve.primes が得られる
template<class T>
std::vector<T> fast_zeta(const std::vector<T>& f, const std::vector<int>& primes) {
    std::vector<T> result = f;
    int n = (int)result.size();
    for (int p : primes) {
        if (p >= n) break;
        for (int k = (n - 1) / p; k >= 1; k--) {
            result[k] += result[k * p];
        }
    }
    return result;
}

// 約数系高速メビウス変換を行う O(N log log N)
// F[k] = sum_{k | i} f[i] を満たす F から f を復元する
// F[0] は使わない
// primes は 2 以上 result.size() - 1 以下の素数列
// 例えば Sieve sieve((int)F.size() - 1); とすると sieve.primes が得られる
template<class T>
std::vector<T> fast_mobius(const std::vector<T>& F, const std::vector<int>& primes) {
    std::vector<T> result = F;
    int n = (int)result.size();
    for (int p : primes) {
        if (p >= n) break;
        for (int k = 1; k * p < n; k++) {
            result[k] -= result[k * p];
        }
    }
    return result;
}
