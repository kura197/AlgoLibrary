#pragma once
// https://github.com/kura197/AlgoLibrary

#include <algorithm>
#include <cassert>
#include <deque>
#include <string>
#include <vector>

using namespace std;

const long long MOD1 = 1000000007LL;
const long long MOD2 = 1000000009LL;
const long long B1 = 1000003LL;
const long long B2 = 1000033LL;

struct Hash {
    long long h1, h2;
    Hash(long long h1 = 0, long long h2 = 0) : h1(h1), h2(h2) {}
    bool operator==(const Hash& other) const {
        return h1 == other.h1 && h2 == other.h2;
    }
};

long long mod_norm(long long x, long long mod){
    x %= mod;
    if(x < 0) x += mod;
    return x;
}

// 蟻p332
// 文字列検索 O(b.length())
// aはbに含まれているか？
bool contain(string a, string b){
    int al = a.length();
    int bl = b.length();
    if(al > bl) return false;

    // Bのal乗を計算
    Hash t(1, 1);
    for(int i = 0; i < al; i++){
        t.h1 = t.h1 * B1 % MOD1;
        t.h2 = t.h2 * B2 % MOD2;
    }

    // aとbの最初のal文字に関するハッシュ値を計算
    Hash ah, bh;
    for(int i = 0; i < al; i++){
        long long ac = static_cast<unsigned char>(a[i]) + 1;
        long long bc = static_cast<unsigned char>(b[i]) + 1;
        ah.h1 = (ah.h1 * B1 + ac) % MOD1;
        ah.h2 = (ah.h2 * B2 + ac) % MOD2;
        bh.h1 = (bh.h1 * B1 + bc) % MOD1;
        bh.h2 = (bh.h2 * B2 + bc) % MOD2;
    }

    // bの場所を1ずつ進めながらハッシュ値をチェック
    for(int i = 0; i + al <= bl; i++){
        if(ah == bh) return true;
        if(i + al < bl){
            long long add = static_cast<unsigned char>(b[i+al]) + 1;
            long long del = static_cast<unsigned char>(b[i]) + 1;
            bh.h1 = mod_norm((bh.h1 * B1 + add) - del * t.h1, MOD1);
            bh.h2 = mod_norm((bh.h2 * B2 + add) - del * t.h2, MOD2);
        }
    }

    return false;
}


// aの末尾とbの先頭を何文字重ねることができるか
int overlap(string a, string b){
    int al = a.length();
    int bl = b.length();
    int ans = 0;
    Hash ah, bh, t(1, 1);
    for(int i = 1; i <= min(al, bl); i++){
        long long ac = static_cast<unsigned char>(a[al-i]) + 1;
        long long bc = static_cast<unsigned char>(b[i-1]) + 1;
        ah.h1 = (ah.h1 + ac * t.h1) % MOD1;
        ah.h2 = (ah.h2 + ac * t.h2) % MOD2;
        bh.h1 = (bh.h1 * B1 + bc) % MOD1;
        bh.h2 = (bh.h2 * B2 + bc) % MOD2;
        if(ah == bh) ans = i;
        t.h1 = t.h1 * B1 % MOD1;
        t.h2 = t.h2 * B2 % MOD2;
    }
    return ans;
}




//////////////////////////////////

/// hash function
Hash get_hash(const vector<long long>& vec){
    Hash hash;
    for(auto x : vec){
        hash.h1 = (hash.h1 * B1 + mod_norm(x, MOD1)) % MOD1;
        hash.h2 = (hash.h2 * B2 + mod_norm(x, MOD2)) % MOD2;
    }
    return hash;
}

/////////////////////////////////

template<long long MOD=1000000009>
struct RollingHashValue {
    long long hash;
    int len;

    RollingHashValue(long long hash = 0, int len = 0) : hash(hash), len(len) {}

    bool operator==(const RollingHashValue& other) const {
        return hash == other.hash && len == other.len;
    }

    bool operator!=(const RollingHashValue& other) const {
        return !(*this == other);
    }
};

template<long long MOD=1000000009>
struct RollingDequeHash {
    long long base;
    long long inv_base;
    mutable vector<long long> pow_base;

    deque<long long> dq;
    RollingHashValue<MOD> value;

    RollingDequeHash(long long base_) : base(base_) {
        inv_base = modinv(base, MOD);
        pow_base.push_back(1);
    }

    static long long modpow(long long a, long long e) {
        long long r = 1;
        while (e > 0) {
            if (e & 1) r = r * a % MOD;
            a = a * a % MOD;
            e >>= 1;
        }
        return r;
    }

    static long long modinv(long long a, long long mod) {
        // MOD が素数なら Fermat でOK
        return modpow(a, mod - 2);
    }

    void ensure_pow(int n) const {
        while ((int)pow_base.size() <= n) {
            pow_base.push_back(pow_base.back() * base % MOD);
        }
    }

    int size() const {
        return value.len;
    }

    bool empty() const {
        return dq.empty();
    }

    RollingHashValue<MOD> get() const {
        return value;
    }

    long long get_hash() const {
        return value.hash;
    }

    RollingHashValue<MOD> concat(const RollingHashValue<MOD>& left, const RollingHashValue<MOD>& right) const {
        ensure_pow(right.len);
        return {
            (left.hash * pow_base[right.len] + right.hash) % MOD,
            left.len + right.len
        };
    }

    RollingHashValue<MOD> concat(const RollingDequeHash& other) const {
        assert(base == other.base);
        return concat(value, other.value);
    }

    void push_back(long long x) {
        // x は 1 以上推奨
        x = mod_norm(x, MOD);
        value.hash = (value.hash * base + x) % MOD;
        value.len++;
        dq.push_back(x);
        ensure_pow(size());
    }

    void push_front(long long x) {
        // x は 1 以上推奨
        x = mod_norm(x, MOD);
        ensure_pow(size());
        value.hash = (x * pow_base[size()] + value.hash) % MOD;
        value.len++;
        dq.push_front(x);
        ensure_pow(size());
    }

    long long pop_back() {
        long long x = dq.back();
        dq.pop_back();

        value.hash = (value.hash - x) % MOD;
        if (value.hash < 0) value.hash += MOD;
        value.hash = value.hash * inv_base % MOD;
        value.len--;

        return x;
    }

    long long pop_front() {
        long long x = dq.front();
        dq.pop_front();

        value.len--;
        ensure_pow(size());
        value.hash = (value.hash - x * pow_base[size()] % MOD) % MOD;
        if (value.hash < 0) value.hash += MOD;

        return x;
    }

    bool operator==(const RollingDequeHash& other) const {
        return value == other.value;
    }

    bool operator!=(const RollingDequeHash& other) const {
        return !(*this == other);
    }
};

// const long long BASE = 911382323;
// RollingDequeHash hash(BASE);
