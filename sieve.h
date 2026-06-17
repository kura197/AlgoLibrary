//エラトステネス
//素数数え上げ
//O(n loglogn) ~ O(n)
// https://atcoder.jp/contests/abc177/tasks/abc177_e


/// prime[x] := x番目の素数
vector<int> prime;
/// is_prime[x] := xが素数かどうか
vector<bool> is_prime;
/// min_prime[x] := xを割り切る最小の素数
vector<int> min_prime;
/// n_prime[x] := xの素因数の種類数
/// TODO: check
vector<int> n_prime;
/// メビウス関数値
vector<int> mobius;

void sieve(int n){
    is_prime.resize(n+1, true);
    min_prime.resize(n+1, -1);
    n_prime.resize(n+1, 0);
    mobius.resize(n+1, 1);
    is_prime[0] = is_prime[1] = false;
    for(int i = 2; i <= n; i++){
        if(is_prime[i]){
            prime.push_back(i);
            min_prime[i] = i;
            n_prime[i] = 1;
            mobius[i] = -1;
            for(int j = 2*i; j <= n; j += i){
                is_prime[j] = false;
                n_prime[j] += 1;
                if(min_prime[j] == -1)
                    min_prime[j] = i;
                if((j/i) % i == 0) mobius[j] = 0;
                else mobius[j] = -mobius[j];
            }
        }
    }
}


/// 素因数分解 : O(log p)
/// 先にsieveを実行する必要あり
map<int, int> prime_factor(int p){
    map<int, int> primes;
    while(p > 1){
        primes[min_prime[p]] += 1;
        p /= min_prime[p];
    }
    return primes;
}

/// 約数列挙 : O(#divs) (#divs <= 1344 for n <= 10^9)
/// 参考: https://qiita.com/drken/items/3beb679e54266f20ab63#4-2-%E9%AB%98%E9%80%9F%E7%B4%84%E6%95%B0%E5%88%97%E6%8C%99
vector<int> divisor(int p){
    auto primes = prime_factor(p);
    vector<int> div{1};
    for(auto& [prime, cnt] : primes){
        int size = div.size();
        for(int i = 0; i < size; i++){
            int x = 1;
            for(int j = 0; j < cnt; j++){
                x *= prime;
                div.push_back(div[i] * x);
            }
        }
    }
    return div;
}

///////////////////////

/// Original

#define MAX_N 1e6

int prime[MAX_N];
bool is_prime[MAX_N+1];

int sieve(int n){
    int p = 0;
    for(int i = 0; i <= n; i++) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for(int i = 2; i <= n; i++){
        if(is_prime[i]){
            prime[p++] = i;
            for(int j = 2*i; j <= n; j += i) is_prime[j] = false;
        }
    }
    return p;
}


