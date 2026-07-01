#include <map>
#include <vector>

// エラトステネスの篩
// 構築 O(n log log n), 素因数分解 O(log n), 約数列挙 O(約数個数)
struct Sieve {
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

    // n以下について素数情報を前計算する O(n log log n)
    void build(int n) {
        primes.clear();
        is_prime.assign(n + 1, true);
        min_prime.assign(n + 1, -1);
        omega.assign(n + 1, 0);
        mobius.assign(n + 1, 1);

        if (n >= 0) is_prime[0] = false;
        if (n >= 1) is_prime[1] = false;

        for (int i = 2; i <= n; i++) {
            if (!is_prime[i]) continue;
            primes.push_back(i);
            min_prime[i] = i;
            omega[i] = 1;
            mobius[i] = -1;

            for (int j = 2 * i; j <= n; j += i) {
                is_prime[j] = false;
                omega[j]++;
                if (min_prime[j] == -1) min_prime[j] = i;
                if ((j / i) % i == 0) mobius[j] = 0;
                else mobius[j] = -mobius[j];
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
