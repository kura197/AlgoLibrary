#pragma once
// https://github.com/kura197/AlgoLibrary

// https://ei1333.hateblo.jp/entry/2017/09/11/211011

#include <algorithm>
#include <cmath>
#include <numeric>
#include <utility>
#include <vector>

using namespace std;

struct Mo {
    int n;  // max val of interval
    vector<pair<int, int>> lr;  // [l, r)

    explicit Mo(int n) : n(n) {}

    void add(int l, int r) {
        lr.emplace_back(l, r);
    }

    template<typename AL, typename AR, typename EL, typename ER, typename O>
    void build(const AL& add_left,
               const AR& add_right,
               const EL& erase_left,
               const ER& erase_right,
               const O& out) {
        int q = (int)lr.size();
        if (q == 0) return;

        int bs = max(1, (int)(n / min<int>(n, sqrt((double)q))));
        vector<int> ord(q);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), [&](int a, int b) {
            int ablock = lr[a].first / bs;
            int bblock = lr[b].first / bs;
            if (ablock != bblock) return ablock < bblock;
            return (ablock & 1) ? lr[a].second > lr[b].second : lr[a].second < lr[b].second;
        });

        int l = 0;
        int r = 0;
        for (int idx : ord) {
            while (l > lr[idx].first) add_left(--l);
            while (r < lr[idx].second) add_right(r++);
            while (l < lr[idx].first) erase_left(l++);
            while (r > lr[idx].second) erase_right(--r);
            out(idx);
        }
    }

    template<typename A, typename E, typename O>
    void build(const A& add, const E& erase, const O& out) {
        build(add, add, erase, erase, out);
    }
};
