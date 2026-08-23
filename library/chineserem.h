#pragma once
// https://github.com/kura197/AlgoLibrary

#include <cassert>
#include <utility>
#include <vector>

using namespace std;

// Reference : https://qiita.com/drken/items/ae02240cd1f8edfc86fd
//
// A = a * x % b を満たす x を求めたいときは
// A + b * y = a * x とみて、a * x - b * y = A を解けばよい
// extGcd(a, b, p, q) で a * p + b * q = d = gcd(a, b) を得たあと、
// A % d == 0 なら p *= A / d, q *= -A / d として 1 つの解が得られる
// x を非負最小にしたいときは x = mod(p, b / d)

#ifndef mod
// 負の数にも対応した mod
inline long long mod(long long a, long long m) {
    return (a % m + m) % m;
}
#endif

// 拡張 Euclid の互除法 O(log max(a, b))
// ap + bq = gcd(a, b) を満たす p, q と gcd(a, b) を返す
long long extGcd(long long a, long long b, long long& p, long long& q) {
    if (b == 0) {
        p = 1;
        q = 0;
        return a;
    }
    long long d = extGcd(b, a % b, q, p);
    q -= a / b * p;
    return d;
}

// 中国剰余定理
// x % m1 == b1 && x % m2 == b2であるxを計算.
// リターン値を (r, m) とすると解は x ≡  r (mod m) (x % m == r)
// 解なしの場合は (0, -1) をリターン
pair<long long, long long> ChineseRem(long long b1, long long m1, long long b2, long long m2) {
    assert(m1 > 0 && m2 > 0);
    b1 = mod(b1, m1);
    b2 = mod(b2, m2);

    long long p, q;
    long long d = extGcd(m1, m2, p, q);
    if ((b2 - b1) % d != 0) return make_pair(0, -1);

    long long m = m1 * (m2 / d);
    long long tmp = (b2 - b1) / d * p % (m2 / d);
    long long r = mod(b1 + m1 * tmp, m);
    return make_pair(r, m);
}

// 全てのidxに対して、
// x % M[idx] == B[idx] を満たすxを計算.
// リターン値を (r, m) とすると解は x ≡  r (mod m) (x % m == r)
// 解なしの場合は (0, -1) をリターン
pair<long long, long long> ChineseRem(const vector<long long>& B, const vector<long long>& M) {
    assert(B.size() == M.size());
    if (B.empty()) return make_pair(0, -1);
    if (B.size() == 1) return make_pair(mod(B[0], M[0]), M[0]);

    auto p = ChineseRem(B[0], M[0], B[1], M[1]);
    for (size_t i = 2; i < B.size(); i++) {
        if (p.second == -1) return p;
        p = ChineseRem(p.first, p.second, B[i], M[i]);
    }
    return p;
}

// xとnが互いで素でない時、逆元が存在しない場合がある(return -1)
// 素数でないnのもとで、xの逆元を計算する
// x * x**(-1) = 1 (mod n)
// x * t = 1 (mod n) となるtを求める
long long inv_mod(long long x, long long n) {
    assert(n > 0);
    long long p, q;
    long long d = extGcd(x, n, p, q);
    if (d != 1 && d != -1) return -1;
    return mod(p, n);
}
