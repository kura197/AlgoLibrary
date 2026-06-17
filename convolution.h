/// 畳み込み演算 f(x) * g(x)
/// O(N*logN) (N > f.size() + g.size())
/// 参考: https://atcoder.jp/contests/atc001/tasks/fft_://atcoder.jp/contests/atc001/tasks/fft_c 

/// ACL (https://atcoder.github.io/ac-library/production/document_ja/convolution.html)
/// -I/path/to/the/ac-library

#include <atcoder/all>

//// Example
//// r + g + b = K
int main(){
    auto RG = atcoder::convolution<MOD>(r, g);
    auto RGB = atcoder::convolution<MOD>(RG, b);
    ll ans = RGB[K];
}






















//////////////////////////////////////

/// TODO: support integer

using Complex = complex<double>;

vector<Complex> dft(const vector<Complex>& A, ll n, int sgn = 1){
    if(n == 1) return A;

    vector<Complex> F(n/2), G(n/2);
    for(int i = 0; i < n/2; i++){
        F[i] = A[2*i + 0];
        G[i] = A[2*i + 1];
    }

    F = dft(F, n/2, sgn);
    G = dft(G, n/2, sgn);

    Complex zeta(cos(2.0 * M_PI / n), sin(2.0 * M_PI / n) * sgn);
    Complex pow_zeta = 1;

    vector<Complex> ret(n);
    for(int i = 0; i < n; i++){
        ret[i] = F[i % (n/2)] + pow_zeta * G[i % (n/2)];
        pow_zeta *= zeta;
    }
    return ret;
}

vector<Complex> inv_dft(const vector<Complex>& A, ll n){
    vector<Complex> ret = dft(A, n, -1);
    for(int i = 0; i < n; i++)
        ret[i] /= n;
    return ret;
}

vector<Complex> convolution(vector<Complex> A, vector<Complex> B){
    int sz = A.size() + B.size() + 1;
    ll n = 1;
    while(n < sz) n *= 2;

    A.resize(n), B.resize(n);
    A = dft(A, n);
    B = dft(B, n);

    vector<Complex> F(n);
    for(int i = 0; i < n; i++){
        F[i] = A[i] * B[i];
    }

    return inv_dft(F, n);
}


//// Example
int main(){
    ll N;
    cin >> N;
    vector<Complex> A(N+1), B(N+1);
    A[0] = B[0] = 0;
    REP(i,N){
        ll a, b;
        cin >> a >> b;
        A[i+1] = Complex(a);
        B[i+1] = Complex(b);
    }

    auto F = convolution(A, B);
    for(int i = 1; i <= 2*N; i++){
        cout << (ll)(F[i].real() + 0.5) << endl;
    }
}
