#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/lazy_segtree_alias/range_add_range_sum.h

#include <vector>

#include <atcoder/lazysegtree>

#include "detail/range_sum_node.h"

template <class T>
RangeSumNode<T> range_add_range_sum_mapping(T f, RangeSumNode<T> x) {
    return {x.sum + f * x.len, x.len};
}

template <class T>
T range_add_range_sum_composition(T f, T g) {
    return f + g;
}

template <class T>
T range_add_range_sum_id() {
    return T{};
}

template <class T>
using RangeAddRangeSumSegtree = atcoder::lazy_segtree<
    RangeSumNode<T>,
    range_sum_op<T>,
    range_sum_e<T>,
    T,
    range_add_range_sum_mapping<T>,
    range_add_range_sum_composition<T>,
    range_add_range_sum_id<T>>;

template <class T>
struct RangeAddRangeSum {
    RangeAddRangeSumSegtree<T> seg;

    RangeAddRangeSum() = default;

    explicit RangeAddRangeSum(int n) : seg(zero_range_sum_nodes<T>(n)) {}

    explicit RangeAddRangeSum(const std::vector<T>& values)
        : seg(range_sum_nodes(values)) {}

    void set(int p, T x) {
        seg.set(p, {x, 1});
    }

    RangeSumNode<T> get_node(int p) {
        return seg.get(p);
    }

    T get(int p) {
        return seg.get(p).sum;
    }

    void apply(int l, int r, T x) {
        seg.apply(l, r, x);
    }

    T prod(int l, int r) {
        return seg.prod(l, r).sum;
    }

    T all_prod() {
        return seg.all_prod().sum;
    }
};
