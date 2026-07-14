#pragma once

#include <numeric>
#include <vector>

using namespace std;

// vector 全体の最小公倍数を返す
// 空 vector に対しては 1 を返す
// 計算量: O(N log max(values))
template<class T>
T lcm(const vector<T>& values) {
    T result = 1;
    for (const T& value : values) {
        result = result / gcd(result, value) * value;
    }
    return result;
}
