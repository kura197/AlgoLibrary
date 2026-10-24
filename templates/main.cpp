/*
https://github.com/kura197/AlgoLibrary
*/

#ifndef ONLINE_JUDGE
#include "pch.hpp"
#else
#pragma GCC optimize "Ofast,omit-frame-pointer,inline,unroll-all-loops"
#include <bits/stdc++.h>
#include <atcoder/all>
#endif

using namespace std;
using namespace atcoder;

using ll = long long;
using ull = unsigned long long;
using int128_t = __int128_t;
using uint128_t = __uint128_t;
using vl = vector<ll>;
using pll = pair<ll, ll>;
template <class Container, class Key>
auto contains(const Container& container, const Key& key)
    -> decltype(container.find(key) != container.end()) {
    return container.find(key) != container.end();
}
template <int m>
ostream& operator<<(ostream& os, const static_modint<m>& x) {
    return os << x.val();
}
template <int id>
ostream& operator<<(ostream& os, const dynamic_modint<id>& x) {
    return os << x.val();
}
#define rep(i, n) for (ll i = 0; i < ll(n); i++)
#define repi(i, a, b) for (ll i = ll(a); i < ll(b); i++)
#define rrep(i, n) for (ll i = ll(n) - 1; i >= 0; i--)
#define rrepi(i, a, b) for (ll i = ll(b) - 1; i >= ll(a); i--)
#define eb emplace_back
#define mems(a, b) memset(a, b, sizeof(a))
#define mp make_pair
#define mt make_tuple
#define mod(a, m) ((a % m + m) % m)
#define bit(x, i) (((x) >> (i)) & 1)
#define all(a) (a).begin(), (a).end()
#define rall(a) (a).rbegin(), (a).rend()
#define IN(y, x, H, W) (0 <= (y) && (y) < (H) && 0 <= (x) && (x) < (W))
#define OUT(y, x, H, W) (!IN(y, x, H, W))
template <class T>
using maxpq = priority_queue<T>;
template <class T>
using minpq = priority_queue<T, vector<T>, greater<T>>;
template <class T>
bool chmax(T& a, const T& b) {
    if (a < b) {
        a = b;
        return 1;
    }
    return 0;
}
template <class T>
bool chmin(T& a, const T& b) {
    if (b < a) {
        a = b;
        return 1;
    }
    return 0;
}
template <class T>
auto make_vec(size_t n) {
    return vector<T>(n);
}
template <class T, class... Sizes>
auto make_vec(size_t n, Sizes... sizes) {
    return vector(n, make_vec<T>(sizes...));
}
template <class Explicit = void, class T, enable_if_t<is_same_v<Explicit, void>, int> = 0>
auto make_vec(size_t n, const T& value) {
    return vector<T>(n, value);
}
template <class Explicit = void, class... Args, enable_if_t<is_same_v<Explicit, void> && (sizeof...(Args) >= 2), int> = 0>
auto make_vec(size_t n, Args... args) {
    return vector(n, make_vec(args...));
}
template <class T>
void rd(T& x);
template <class T, class U>
void rd(pair<T, U>& p);
template <class... Ts>
void rd(tuple<Ts...>& t);
template <class T>
void rd(vector<T>& v);
template <class T, class U, class... Ts>
void rd(T& x, U& y, Ts&... xs);
template <class T>
void rd(T& x) {
    cin >> x;
}
template <class T, class U>
void rd(pair<T, U>& p) {
    rd(p.first);
    rd(p.second);
}
template <class... Ts>
void rd(tuple<Ts...>& t) {
    apply([](auto&... xs) { (rd(xs), ...); }, t);
}
template <class T>
void rd(vector<T>& v) {
    for (auto& x : v) rd(x);
}
template <class T, class U, class... Ts>
void rd(T& x, U& y, Ts&... xs) {
    rd(x);
    rd(y);
    (rd(xs), ...);
}
template <class T>
void wr(const vector<T>& v) {
    for (int i = 0; i < int(v.size()); i++) cout << v[i] << " \n"[i + 1 == int(v.size())];
}
template <class T>
void wrl(const vector<T>& v) {
    for (const auto& x : v) cout << x << '\n';
}
template <class T>
void dec(T& x);
template <class T>
void dec(vector<T>& v);
template <class T, class U>
void dec(pair<T, U>& p);
template <class... Ts>
void dec(tuple<Ts...>& t);
template <class T>
void inc(T& x);
template <class T>
void inc(vector<T>& v);
template <class T, class U>
void inc(pair<T, U>& p);
template <class... Ts>
void inc(tuple<Ts...>& t);
template <class T>
void dec(T& x) {
    --x;
}
template <class T>
void dec(vector<T>& v) {
    for (auto& x : v) dec(x);
}
template <class T, class U>
void dec(pair<T, U>& p) {
    dec(p.first);
    dec(p.second);
}
template <class... Ts>
void dec(tuple<Ts...>& t) {
    apply([](auto&... xs) { (dec(xs), ...); }, t);
}
template <class T>
void inc(T& x) {
    ++x;
}
template <class T>
void inc(vector<T>& v) {
    for (auto& x : v) inc(x);
}
template <class T, class U>
void inc(pair<T, U>& p) {
    inc(p.first);
    inc(p.second);
}
template <class... Ts>
void inc(tuple<Ts...>& t) {
    apply([](auto&... xs) { (inc(xs), ...); }, t);
}
template <class T>
void uniq(vector<T>& v) {
    sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
}
template <class T>
vector<T> get_sum_vector(const vector<T>& v) {
    vector<T> sum(v.size() + 1);
    for (int i = 0; i < int(v.size()); i++) sum[i + 1] = sum[i] + v[i];
    return sum;
}

#ifdef ONLINE_JUDGE
#define DEBUG(...) ((void)0)
#define DUMP(...) ((void)0)
#define ASSERT(...) ((void)0)
#else
inline bool debug_enabled() {
    const char* env = std::getenv("DISABLE_DEBUG");
    return env == nullptr || env[0] == '\0' || std::strcmp(env, "0") == 0;
}
#define DEBUG(...)                                        \
    do {                                                  \
        if (debug_enabled()) { ::debug_utils::print(std::cerr, __VA_ARGS__); } \
    } while (false)
#define DUMP(...)                                        \
    do {                                                 \
        if (debug_enabled()) { ::debug_utils::dump(std::cerr, #__VA_ARGS__, __VA_ARGS__); } \
    } while (false)
#define ASSERT(expr, ...)                                                                               \
    do {                                                                                                \
        if (!(expr)) { ::debug_utils::fail_assertion(std::cerr, #expr, __FILE__, __LINE__, __func__, __VA_ARGS__); } \
    } while (false)
#endif  // ONLINE_JUDGE

// R, D, L, U
constexpr int dx[] = {1, 0, -1, 0};
constexpr int dy[] = {0, 1, 0, -1};

const ll MOD = 998244353;
using mint = modint998244353;

void print_yes_no(bool valid) {
    if (valid) cout << "Yes" << endl;
    else cout << "No" << endl;
}

//#include ""

int main() {
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);



    return 0;
}
