#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/runlength.h

#include <string>
#include <utility>
#include <vector>

using namespace std;

// 連続する同じ文字を (文字, 連続数) に圧縮する: O(N)
// 例: "aaabbc" -> {{'a', 3}, {'b', 2}, {'c', 1}}
vector<pair<char, int> > runlength(const string& value) {
    vector<pair<char, int> > result;
    for (char c : value) {
        if (result.empty() || result.back().first != c) {
            result.push_back(make_pair(c, 1));
        } else {
            result.back().second++;
        }
    }
    return result;
}
