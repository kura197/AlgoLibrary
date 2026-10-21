#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/lazy_segtree_alias/range_assign_range_sum.h

#include <vector>

#include <atcoder/lazysegtree>

#include "detail/range_assign.h"
#include "detail/range_sum_node.h"

template <class T>
RangeSumNode<T> range_assign_range_sum_mapping(
    RangeAssignValue<T> f,
    RangeSumNode<T> x
) {
    if (!f.has_value) return x;
    return {f.value * x.len, x.len};
}

template <class T>
using RangeAssignRangeSumSegtree = atcoder::lazy_segtree<
    RangeSumNode<T>,
    range_sum_op<T>,
    range_sum_e<T>,
    RangeAssignValue<T>,
    range_assign_range_sum_mapping<T>,
    range_assign_composition<T>,
    range_assign_id<T>>;

template <class T>
struct RangeAssignRangeSum {
    RangeAssignRangeSumSegtree<T> seg;

    RangeAssignRangeSum() = default;

    explicit RangeAssignRangeSum(int n) : seg(zero_range_sum_nodes<T>(n)) {}

    explicit RangeAssignRangeSum(const std::vector<T>& values)
        : seg(range_sum_nodes(values)) {}

    void set(int p, T x) {
        seg.set(p, {x, 1});
    }

    T get(int p) {
        return seg.get(p).sum;
    }

    void apply(int l, int r, T x) {
        seg.apply(l, r, {x, true});
    }

    T prod(int l, int r) {
        return seg.prod(l, r).sum;
    }

    T all_prod() {
        return seg.all_prod().sum;
    }
};
