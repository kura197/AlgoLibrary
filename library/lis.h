#pragma once

#include <algorithm>
#include <vector>

using namespace std;

/// 最長増加部分列 O(N)
/// 要確認

/// get length of longest increasing subsequence of vector A
int get_lis_length(vector<long long>& A){
    const long long INF = 1e15;
    int N = (int)A.size();
    vector<long long> dp(N, INF);
    for (int i = 0; i < N; i++) {
        auto a = A[i];
        //// 狭義増加部分列の場合: <- 要確認
        //// auto iter = lower_bound(dp.begin(), dp.end(), a);
        auto iter = upper_bound(dp.begin(), dp.end(), a);
        *iter = a;
    }

    long long ret = lower_bound(dp.begin(), dp.end(), INF) - dp.begin();
    return ret;
}
