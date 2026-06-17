//漸化式 O(logn)
//ABC 009D
//半環であれば計算可能(+ -> ^ / * -> &)

using vec = vector<ll>;
using mat = vector<vec>;

//単位元
const int ID = 1;

void print_mat(mat A){
    for(size_t i = 0; i < A.size(); i++){
        for(size_t j = 0; j < A[0].size(); j++){
            printf("%lld ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_vec(vec A){
    for(size_t i = 0; i < A.size(); i++){
        printf("%lld ", A[i]);
    }
    printf("\n\n");
}

/// matrix-matrix multiplication
mat mul(mat& A, mat& B){
    mat C(A.size(), vec(B[0].size()));
    for(size_t i = 0; i < A.size(); i++){
        for(size_t k = 0; k < B.size(); k++){
            for(size_t j = 0; j < B[0].size(); j++){
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]);
                C[i][j] %= MOD;
            }
        }
    }
    return C;
}

/// matrix-vector multiplication
vec mul(mat& A, vec& b){
    assert(A[0].size() == b.size());
    vec c(b.size());
    for(size_t i = 0; i < A.size(); i++){
        for(size_t j = 0; j < b.size(); j++){
            c[i] += A[i][j] * b[j];
            c[i] %= MOD;
        }
    }
    return c;
}

//calc A**n
mat pow(mat A, ll n){
    mat B(A.size(), vec(A.size()));
    for(size_t i = 0; i < A.size(); i++){
        B[i][i] = ID;
    }
    while(n > 0){
        if(n & 1) B = mul(B, A);
        A = mul(A, A);
        n >>= 1;
    }
    return B;
}
