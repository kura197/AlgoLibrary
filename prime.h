
////素数判定 O(n**1/2)
bool is_prime(int n){
    for(ll i = 2; i * i <= n; i++){
        if(n % i == 0) return false;
    }
    return n != 1;
}

#include <vector>

////約数列挙 O(n**1/2)
vector<int> divisor(int n){
    vector<int> res;
    for(ll i = 1; i * i <= n; i++){
        if(n % i == 0){
            res.push_back(i);
            if(i != n/i) res.push_back(n/i);
        }
    }
    return res;
}

#include <map>

////素因数分解 O(n**1/2)
map<int, int> prime_factor(int n){
    map<int, int> res;
    for(ll i = 2; i * i <= n; i++){
        while(n % i == 0){
            ++res[i];
            n /= i;
        }
    }
    if(n != 1) res[n] = 1;
    return res;
}

////n以下の素数の数を返す O(n log(logn)) ~~ O(n)
#define MAX_N 10000
//i番目の素数
int prime[MAX_N];
//trueなら素数
bool is_prime[MAX_N + 1];

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

////高速素因数分解 O(AloglogA + NlogA) *under test (ABC 177E)

#define MAX_A 1001000

// small_prime[x] : xを割り切る最小の素数 
ll small_prime[MAX_A];

// small_prime[x]の計算. O(AloglogA)
void sieve(){
    for(int a = 2; a  < MAX_A; a++){
        if(small_prime[a] != 0) continue;
        small_prime[a] = a;

        for(int t = a+a; t < MAX_A; t += a){
            if(small_prime[t] == 0) 
                small_prime[t] = a;
        }
    }
}

// 素因数分解. O(NlogA)
map<int, int> prime_factor(int n){
    map<int, int> res;
    while(n != 1){
        res[X[n]]++;
        n /= X[n];
    }
    return res;
}
