#pragma once

#include <limits>
#include <unordered_map>

using namespace std;

class SparsePredecessorDSU {
private:
    unordered_map<long long, long long> parent;

public:
    // 左端の番兵.
    // この値自体は管理対象として使わず, 「存在しない」を表すために使う.
    static constexpr long long begin_sentinel() {
        return numeric_limits<long long>::min();
    }

    // x 以下でまだ消されていない最大値を返す
    long long find(long long x) {
        if (x <= begin_sentinel()) return begin_sentinel();

        auto it = parent.find(x);
        if (it == parent.end()) {
            return x;
        }

        return it->second = find(it->second);
    }

    // x を消す
    void erase(long long x) {
        if (x == begin_sentinel()) return;
        parent[x] = find(x - 1);
    }

    long long prev(long long x) {
        return find(x);
    }

    bool erased(long long x) {
        if (x == begin_sentinel()) return true;
        return find(x) != x;
    }

    bool contains(long long x) {
        if (x == begin_sentinel()) return false;
        return find(x) == x;
    }
};

class SparseSuccessorDSU {
private:
    unordered_map<long long, long long> parent;

public:
    // 右端の番兵.
    // この値自体は管理対象として使わず, 「存在しない」を表すために使う.
    static constexpr long long end_sentinel() {
        return numeric_limits<long long>::max();
    }

private:
    long long find(long long x) {
        if (x >= end_sentinel()) {
            return end_sentinel();
        }

        auto it = parent.find(x);
        if (it == parent.end()) {
            return x;
        }

        return it->second = find(it->second);
    }

public:
    // x を削除する
    // すでに削除済みでも問題ない
    void erase(long long x) {
        if (x == end_sentinel()) return;
        parent[x] = find(x + 1);
    }

    // x 以上で削除されていない最小値を返す
    // 存在しなければ end_sentinel() を返す
    long long next(long long x) {
        return find(x);
    }

    bool contains(long long x) {
        if (x == end_sentinel()) return false;
        return find(x) == x;
    }

    bool erased(long long x) {
        if (x == end_sentinel()) return true;
        return !contains(x);
    }
};
