#include <map>
#include <vector>

// 素数かどうかを判定する O(sqrt(n))
bool is_prime(long long n){
    if(n <= 1) return false;
    for(long long i = 2; i * i <= n; i++){
        if(n % i == 0) return false;
    }
    return true;
}

// 約数を列挙する O(sqrt(n))
std::vector<long long> divisor(long long n){
    std::vector<long long> res;
    for(long long i = 1; i * i <= n; i++){
        if(n % i == 0){
            res.push_back(i);
            if(i != n / i) res.push_back(n / i);
        }
    }
    return res;
}

// 素因数分解する O(sqrt(n))
std::map<long long, int> prime_factor(long long n){
    std::map<long long, int> res;
    for(long long i = 2; i * i <= n; i++){
        while(n % i == 0){
            res[i]++;
            n /= i;
        }
    }
    if(n != 1) res[n]++;
    return res;
}
