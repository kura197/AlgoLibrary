#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/lazy_segtree_alias/range_affine_range_sum.h

#include <vector>

#include <atcoder/lazysegtree>

#include "detail/affine_transformation.h"
#include "detail/range_sum_node.h"

template <class T>
RangeSumNode<T> range_affine_range_sum_mapping(
    AffineTransformation<T> f,
    RangeSumNode<T> x
) {
    return {f.a * x.sum + f.b * x.len, x.len};
}

template <class T>
using RangeAffineRangeSumSegtree = atcoder::lazy_segtree<
    RangeSumNode<T>,
    range_sum_op<T>,
    range_sum_e<T>,
    AffineTransformation<T>,
    range_affine_range_sum_mapping<T>,
    affine_composition<T>,
    affine_id<T>>;

template <class T>
struct RangeAffineRangeSum {
    RangeAffineRangeSumSegtree<T> seg;

    RangeAffineRangeSum() = default;

    explicit RangeAffineRangeSum(int n) : seg(zero_range_sum_nodes<T>(n)) {}

    explicit RangeAffineRangeSum(const std::vector<T>& values)
        : seg(range_sum_nodes(values)) {}

    void set(int p, T x) {
        seg.set(p, {x, 1});
    }

    T get(int p) {
        return seg.get(p).sum;
    }

    void apply(int l, int r, AffineTransformation<T> f) {
        seg.apply(l, r, f);
    }

    void apply(int l, int r, T a, T b) {
        apply(l, r, {a, b});
    }

    T prod(int l, int r) {
        return seg.prod(l, r).sum;
    }

    T all_prod() {
        return seg.all_prod().sum;
    }
};
