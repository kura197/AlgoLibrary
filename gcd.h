#pragma once

// 拡張Euclidの互除法 O(log max(a, b))
// ax + by = gcd(a, b) を満たす x, y と gcd(a, b) を返す
long long extGCD(long long a, long long b, long long& x, long long& y){
    if(b == 0){
        x = 1;
        y = 0;
        return a;
    }

    long long d = extGCD(b, a % b, y, x);
    y -= (a / b) * x;
    return d;
}
