#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/pow.h

#include <cassert>
#include <functional>

// 結合的な演算 operation による base の exponent 乗を求める.
// identity は operation の単位元とする. 計算量 O(log exponent).
template<class T, class Operation>
T binary_power(
    T base,
    long long exponent,
    T identity,
    Operation operation
) {
    assert(exponent >= 0);
    T result = identity;
    while (exponent > 0) {
        if (exponent & 1LL) result = operation(result, base);
        base = operation(base, base);
        exponent >>= 1;
    }
    return result;
}

// operator* による base の exponent 乗を求める.
// T{1} が乗算の単位元になる型に対して使用する.
template<class T>
T binary_power(T base, long long exponent) {
    return binary_power(base, exponent, T{1}, std::multiplies<T>());
}

// base^exponent mod mod を求める. 計算量 O(log exponent).
// 乗算には __int128 を使うため、mod は long long の正の範囲で指定できる.
inline long long mod_power(long long base, long long exponent, long long mod) {
    assert(exponent >= 0);
    assert(mod > 0);

    base %= mod;
    if (base < 0) base += mod;
    long long result = 1 % mod;

    while (exponent > 0) {
        if (exponent & 1LL) {
            result = (long long)((__int128)result * base % mod);
        }
        base = (long long)((__int128)base * base % mod);
        exponent >>= 1;
    }
    return result;
}

// 繰り返し二乗法
inline long long mypow(long long base, long long exponent) {
    return binary_power(base, exponent);
}
