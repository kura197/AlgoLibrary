#pragma once

#include <algorithm>
#include <utility>
#include <vector>

using namespace std;

template<typename T>
struct Trace {
    vector<pair<T, int>> log;

    Trace() = default;

    unsigned int add(T c, int id) {
        log.emplace_back(c, id);
        return (unsigned int)log.size() - 1;
    }

    vector<T> get(int id) const {
        vector<T> ret;
        while (id != -1) {
            const auto& [c, next_id] = log[id];
            ret.emplace_back(c);
            id = next_id;
        }
        reverse(ret.begin(), ret.end());
        return ret;
    }
};
