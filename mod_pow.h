#pragma once



///////////////////////////////
//// MOD固定. 少し速い (E.g. 2000ms --> 1900ms)

//O(logn)
//return x**n % MOD
long long mod_pow(long long x, long long n){
    x %= MOD;
    long long res = 1;
    while(n > 0){
        if(n & 1) res = res * x % MOD;
        x = x * x % MOD;
        n >>= 1;
    }
    return res;
}

//return x / y % MOD
long long mod_div(long long x, long long y){
    long long ret = x * mod_pow(y, MOD-2);
    return ret % MOD;
}


///////////////////////////////

//O(logn)
//return x**n % mod
long long mod_pow(long long x, long long n, long long mod){
    x %= mod;
    long long res = 1;
    while(n > 0){
        if(n & 1) res = res * x % mod;
        x = x * x % mod;
        n >>= 1;
    }
    return res;
}

//return x / y % MOD
long long mod_div(long long x, long long y, long long mod){
    long long ret = x * mod_pow(y, mod-2, mod);
    return ret % mod;
}
