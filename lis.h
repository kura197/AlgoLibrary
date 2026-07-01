#pragma once

/// 最長増加部分列 O(N)
/// 要確認

/// get length of longest increasing subsequence of vector A
int get_lis_length(vector<ll>& A){
    const ll INF = 1e15;
    vector<ll> dp(N, INF);
    REP(i,N){
        auto a = A[i];
        //// 狭義増加部分列の場合: <- 要確認
        //// auto iter = lower_bound(dp.begin(), dp.end(), a);
        auto iter = upper_bound(dp.begin(), dp.end(), a);
        *iter = a;
    }

    ll ret = lower_bound(dp.begin(), dp.end(), INF) - dp.begin();
    return ret;
}
