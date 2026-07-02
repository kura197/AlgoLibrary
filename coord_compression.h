#pragma once

#include <algorithm>
#include <utility>
#include <vector>

using namespace std;

// return {compressed vector, original values}
// xs[compressed[i]] == vec[i]
pair<vector<int>, vector<long long>> coord_compression(const vector<long long>& vec) {
    vector<long long> xs = vec;
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());

    vector<int> compressed(vec.size());
    for (int i = 0; i < (int)vec.size(); i++) {
        compressed[i] = lower_bound(xs.begin(), xs.end(), vec[i]) - xs.begin();
    }

    return {compressed, xs};
}
