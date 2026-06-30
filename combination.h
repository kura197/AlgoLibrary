

//MODありの場合 (ACL)
//------------------------------------------------
using mint = modint998244353;

vector<mint> fact;
vector<mint> frev;

void prepare(int size){
    fact.assign(size + 1, 1);
    frev.assign(size + 1, 1);
    for(int i = 1; i <= size; i++){
        fact[i] = fact[i-1] * i;
    }
    frev[size] = fact[size].inv();
    for(int i = size; i >= 1; i--){
        frev[i-1] = frev[i] * i;
    }
}

ll comb(int n, int k){
    if(n < 0 || k < 0 || n < k) return 0;
    if(n == 0 || k == 0) return 1;
    return (fact[n] * frev[k] * frev[n-k]).val();
}

ll perm(int n, int k){
    if(n < 0 || k < 0 || n < k) return 0;
    if(n == 0 || k == 0) return 1;
    return (fact[n] * frev[n-k]).val();
}

//in main function
prepare(size);

//------------------------------------------------

//MODありの場合
//------------------------------------------------
const ll MOD = 1e9+7;
const int MAX_N = 200000;

ll fact[MAX_N+100];
ll frev[MAX_N+100];

ll rev(ll a, ll p){
    if(p == 0) return 1;
    ll tmp = rev(a, p/2);
    tmp = tmp * tmp % MOD;
    if(p % 2 == 1) tmp = tmp * a % MOD;
    return tmp;
}

ll comb(int n, int k){
    if(n < 0 || k < 0 || n < k) return 0;
    if(n == 0 || k == 0) return 1;
    return fact[n] * frev[k] % MOD * frev[n-k] % MOD;
}

ll perm(int n, int k){
    if(n < 0 || k < 0 || n < k) return 0;
    if(n == 0 || k == 0) return 1;
    return fact[n] * frev[n-k] % MOD;
}

void prepare(){
    fact[0] = frev[0] = 1;
    for(int i = 1; i <= MAX_N; i++){
        fact[i] = (fact[i-1] * i) % MOD;
        frev[i] = rev(fact[i], MOD-2);
    }
}

//in main function
prepare();

//------------------------------------------------


//逐次計算
ll C[51][51];
void comb_table(int N){
    for(int i = 0; i <= N; i++){
        for(int j = 0; j <= i; j++){
            if(j == 0 || j == i)
                C[i][j] = 1LL;
            else
                C[i][j] = C[i-1][j-1] + C[i-1][j];
        }
    }
}

//--------------------------------------------

ll mod_pow(ll x, ll n, ll mod){
    ll res = 1;
    while(n > 0){
        if(n & 1) res = res * x % mod;
        x = x * x % mod;
        n >>= 1;
    }
    return res;
}

//要確認 MOD
//O(b)
ll comb_mod(ll a, ll b, ll mod){
    ll num = 1;
    ll div = 1;
    for(int i = 0; i < b; i++){
        num = (num * (a-i)) % mod;
        div = (div * (b-i)) % mod;
    }

    ll ret = (num * mod_pow(div, mod-2, mod)) % mod;
    return ret;
}


//-----------------------------------
//TODO: WIP
//
template<int N>
struct Comb {
    ll fact[N+100];
    ll frev[N+100];

    constexpr Comb() : fact(), frev() {
        prepare();
    }

    constexpr ll rev(ll a, ll p){
        if(p == 0) return 1;
        ll tmp = rev(a, p/2);
        tmp = tmp * tmp % MOD;
        if(p % 2 == 1) tmp = tmp * a % MOD;
        return tmp;
    }

    constexpr void prepare(){
        fact[0] = frev[0] = 1;
        for(int i = 1; i <= N; i++){
            fact[i] = (fact[i-1] * i) % MOD;
            frev[i] = rev(fact[i], MOD-2);
        }
    }

    ll comb(int n, int k){
        if(n < 0 || k < 0 || n < k) return 0;
        if(n == 0 || k == 0) return 1;
        return fact[n] * frev[k] % MOD * frev[n-k] % MOD;
    }

    ll perm(int n, int k){
        if(n < 0 || k < 0 || n < k) return 0;
        if(n == 0 || k == 0) return 1;
        return fact[n] * frev[n-k] % MOD;
    }
};
