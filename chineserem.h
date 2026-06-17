//// Reference : https://qiita.com/drken/items/ae02240cd1f8edfc86fd


// 負の数にも対応した mod
// 例えば -17 を 5 で割った余りは本当は 3 (-17 ≡ 3 (mod. 5))
// しかし単に -17 % 5 では -2 になってしまう
inline long long mod(long long a, long long m) {
    return (a % m + m) % m;
}

// 拡張 Euclid の互除法
// ap + bq = gcd(a, b) となる (p, q) を求め、d = gcd(a, b) をリターンします
//
// ## TODO : 要確認
// ## A = a*x % b を満たすxを求める 
// A = a*x % b --> A + b*y = a*x -->  a*x - b*y = A
// ## ax + by = Aを満たす一般解(x, y)を求める
// auto d = extGcd(a, b, p, q);
// if(A % d != 0) continue;
// p *= A/d, q *= A/d;
// (x, y) = (p + k * (b/d), q - k * (a/d));
//
// (x >= 0となる解)
// x = mod(p, b/d);
// ll k = (p-x) / (b/d);
// y = q - k * (a/d);
long long extGcd(long long a, long long b, long long &p, long long &q) {  
    if (b == 0) { p = 1; q = 0; return a; }  
    long long d = extGcd(b, a%b, q, p);  
    q -= a/b * p;  
    return d;  
}

// 中国剰余定理
// x % m1 == b1 && x % m2 == b2であるxを計算.
// リターン値を (r, m) とすると解は x ≡  r (mod m) (x % m == r)
// 解なしの場合は (0, -1) をリターン
pair<long long, long long> ChineseRem(long long b1, long long m1, long long b2, long long m2) {
  long long p, q;
  long long d = extGcd(m1, m2, p, q); // p is inv of m1/d (mod. m2/d)
  if ((b2 - b1) % d != 0) return make_pair(0, -1);
  long long m = m1 * (m2/d); // lcm of (m1, m2)
  long long tmp = (b2 - b1) / d * p % (m2/d);
  long long r = mod(b1 + m1 * tmp, m);
  return make_pair(r, m);
}

// ##TODO: 要確認
// 全てのidxに対して、
// x % M[idx] == B[idx] を満たすxを計算.
// リターン値を (r, m) とすると解は x ≡  r (mod m) (x % m == r)
// 解なしの場合は (0, -1) をリターン
pair<long long, long long> ChineseRem(vector<long long>& B, vector<long long>& M) {
    assert(B.size() == M.size());
    if(B.size() == 0)
        return make_pair(0, -1);
    if(B.size() == 1)
        return make_pair(B[0], M[0]);

    auto p = ChineseRem(B[0], M[0], B[1], M[1]);
    for(size_t i = 2; i < B.size(); i++){
        if(p.second == -1)
            return p;
        p = ChineseRem(p.first, p.second, B[i], M[i]);
    }
    return p;
}

// ##TODO: 要確認 (https://atcoder.jp/contests/abc186/submissions/24065332)
// xとnが互いで素でない時、逆元が存在しない場合がある(return -1)
//
// 素数でないnのもとで、xの逆元を計算する
// x * x**(-1) = 1 (mod n)
// x * t = 1 (mod n) となるtを求める
//
// X = 1 (mod n)
// X = 0 (mod x)
// これらを満たすXを求め、xで割る
long long inv_mod(long long x, long long n){
    auto [r, m] = ChineseRem(1, n, 0, x);
    if(m == -1)
        return -1;
    else
        return r / x;
}
