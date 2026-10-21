#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/lazy_segtree_alias/range_add_range_max.h

#include <algorithm>
#include <limits>
#include <vector>

#include <atcoder/lazysegtree>

template <class T>
T range_add_range_max_op(T a, T b) {
    return std::max(a, b);
}

template <class T>
T range_add_range_max_e() {
    return std::numeric_limits<T>::lowest();
}

template <class T>
T range_add_range_max_mapping(T f, T x) {
    if (x == range_add_range_max_e<T>()) return x;
    return x + f;
}

template <class T>
T range_add_range_max_composition(T f, T g) {
    return f + g;
}

template <class T>
T range_add_range_max_id() {
    return T{};
}

template <class T>
using RangeAddRangeMaxSegtree = atcoder::lazy_segtree<
    T,
    range_add_range_max_op<T>,
    range_add_range_max_e<T>,
    T,
    range_add_range_max_mapping<T>,
    range_add_range_max_composition<T>,
    range_add_range_max_id<T>>;

template <class T>
struct RangeAddRangeMax {
    RangeAddRangeMaxSegtree<T> seg;

    RangeAddRangeMax() = default;

    explicit RangeAddRangeMax(int n) : seg(n) {}

    explicit RangeAddRangeMax(const std::vector<T>& values) : seg(values) {}

    void set(int p, T x) {
        seg.set(p, x);
    }

    T get(int p) {
        return seg.get(p);
    }

    void apply(int l, int r, T x) {
        seg.apply(l, r, x);
    }

    T prod(int l, int r) {
        return seg.prod(l, r);
    }

    T all_prod() {
        return seg.all_prod();
    }
};
