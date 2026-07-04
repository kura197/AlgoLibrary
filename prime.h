#pragma once

#include <map>
#include <vector>

// 素数かどうかを判定する O(sqrt(n))
bool is_prime(long long n){
    if(n <= 1) return false;
    for(long long i = 2; i * i <= n; i++){
        if(n % i == 0) return false;
    }
    return true;
}

// 約数を列挙する O(sqrt(n))
std::vector<long long> divisor(long long n){
    std::vector<long long> res;
    for(long long i = 1; i * i <= n; i++){
        if(n % i == 0){
            res.push_back(i);
            if(i != n / i) res.push_back(n / i);
        }
    }
    return res;
}

// 素因数分解する O(sqrt(n))
std::map<long long, int> prime_factor(long long n){
    std::map<long long, int> res;
    for(long long i = 2; i * i <= n; i++){
        while(n % i == 0){
            res[i]++;
            n /= i;
        }
    }
    if(n != 1) res[n]++;
    return res;
}

// オイラーのトーシェント関数 phi(n) を計算する O(sqrt(n))
// 1 以上 n 以下の整数のうち、n と互いに素なものの個数を返す
long long totient(long long n) {
    long long ret = n;
    for (long long p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            ret -= ret / p;
            while (n % p == 0) n /= p;
        }
    }
    if (n != 1) ret -= ret / n;
    return ret;
}
