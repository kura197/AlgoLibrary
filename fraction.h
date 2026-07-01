#pragma once

/// 有理数型 (doubleなどの誤差が問題になる場合に使用)
/// 参考: https://atcoder.jp/contests/abc225/editorial/2853
/// 第一象限以外で使用できるか不明
/// (x/y)の大小で順番を規定

struct fraction{
    ll p,q;
    fraction(ll P = 0, ll Q = 1): p(P), q(Q){}
    bool operator<(const fraction &other)const{
        return p*other.q < other.p*q;
    }
    bool operator<=(const fraction &other)const{
        return p*other.q <= other.p*q;
    }
};
