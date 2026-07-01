#pragma once


///////////////////////////////
//// MOD固定. 少し速い (E.g. 2000ms --> 1900ms)

//O(logn)
//return x**n % MOD
ll mod_pow(ll x, ll n){
    x %= MOD;
    ll res = 1;
    while(n > 0){
        if(n & 1) res = res * x % MOD;
        x = x * x % MOD;
        n >>= 1;
    }
    return res;
}

//return x / y % MOD
ll mod_div(ll x, ll y){
    ll ret = x * mod_pow(y, MOD-2);
    return ret % MOD;
}


///////////////////////////////

//O(logn)
//return x**n % mod
ll mod_pow(ll x, ll n, ll mod){
    x %= mod;
    ll res = 1;
    while(n > 0){
        if(n & 1) res = res * x % mod;
        x = x * x % mod;
        n >>= 1;
    }
    return res;
}

//return x / y % MOD
ll mod_div(ll x, ll y, ll mod){
    ll ret = x * mod_pow(y, mod-2, mod);
    return ret % mod;
}
