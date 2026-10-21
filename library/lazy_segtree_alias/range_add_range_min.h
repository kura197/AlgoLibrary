#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/lazy_segtree_alias/range_add_range_min.h

#include <algorithm>
#include <limits>
#include <vector>

#include <atcoder/lazysegtree>

template <class T>
T range_add_range_min_op(T a, T b) {
    return std::min(a, b);
}

template <class T>
T range_add_range_min_e() {
    return std::numeric_limits<T>::max();
}

template <class T>
T range_add_range_min_mapping(T f, T x) {
    if (x == range_add_range_min_e<T>()) return x;
    return x + f;
}

template <class T>
T range_add_range_min_composition(T f, T g) {
    return f + g;
}

template <class T>
T range_add_range_min_id() {
    return T{};
}

template <class T>
using RangeAddRangeMinSegtree = atcoder::lazy_segtree<
    T,
    range_add_range_min_op<T>,
    range_add_range_min_e<T>,
    T,
    range_add_range_min_mapping<T>,
    range_add_range_min_composition<T>,
    range_add_range_min_id<T>>;

template <class T>
struct RangeAddRangeMin {
    RangeAddRangeMinSegtree<T> seg;

    RangeAddRangeMin() = default;

    explicit RangeAddRangeMin(int n) : seg(n) {}

    explicit RangeAddRangeMin(const std::vector<T>& values) : seg(values) {}

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
