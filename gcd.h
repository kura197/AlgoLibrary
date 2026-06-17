
ll gcd(ll a, ll b){
    if(b == 0) return a;
    return gcd(b,a % b);
}

// ABC186 E
// O(log max(a, b) ?)
// ax + by = gcd(a, b)を満たす(x, y)
ll extGCD(ll a, ll b, ll& x, ll& y){
    if(b == 0){
        // dx + 0y = d
        x = 1;
        y = 0;
        return a;
    }
    else{
        ll d = extGCD(b, a%b, y, x);
        y -= (a/b) * x;
        return d;
    }
}


//// ax + by = d の一般解
ll g = gcd(a, b);
if(d % g != 0){
    // 解なし
    return -1;
}
else{
    ll tmpx, tmpy;
    extGCD(a, b, tmpx, tmpy);
    x = b / g + tmpx;
    y = a / g + tmpy;
}


// 負の数に対応したmod
inline ll mod(ll a, ll m){
    return (a % m + m) % m;
}

// aの逆元を求める。(gcd(a, m) == 1)
// ax - my = 1
ll mod_inv(ll a, ll m){
    ll x, y;
    extGCD(a, m, x, y);
    //y *= -1;
    //ll inv = m * ((-x + m - 1)/m) + x;
    ll inv = mod(x, m);
    return inv;
}
