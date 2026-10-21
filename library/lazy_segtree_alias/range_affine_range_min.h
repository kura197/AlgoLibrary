#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/lazy_segtree_alias/range_affine_range_min.h

#include <algorithm>
#include <vector>

#include <atcoder/lazysegtree>

#include "detail/affine_transformation.h"
#include "detail/range_min_max_node.h"

template <class T>
RangeMinMaxNode<T> range_affine_range_min_mapping(
    AffineTransformation<T> f,
    RangeMinMaxNode<T> x
) {
    if (x.len == 0) return x;
    T first = f.a * x.min + f.b;
    T second = f.a * x.max + f.b;
    return {std::min(first, second), std::max(first, second), x.len};
}

template <class T>
using RangeAffineRangeMinSegtree = atcoder::lazy_segtree<
    RangeMinMaxNode<T>,
    range_min_max_op<T>,
    range_min_max_e<T>,
    AffineTransformation<T>,
    range_affine_range_min_mapping<T>,
    affine_composition<T>,
    affine_id<T>>;

template <class T>
struct RangeAffineRangeMin {
    RangeAffineRangeMinSegtree<T> seg;

    RangeAffineRangeMin() = default;

    explicit RangeAffineRangeMin(int n) : seg(zero_range_min_max_nodes<T>(n)) {}

    explicit RangeAffineRangeMin(const std::vector<T>& values)
        : seg(range_min_max_nodes(values)) {}

    void set(int p, T x) {
        seg.set(p, {x, x, 1});
    }

    T get(int p) {
        return seg.get(p).min;
    }

    void apply(int l, int r, AffineTransformation<T> f) {
        seg.apply(l, r, f);
    }

    void apply(int l, int r, T a, T b) {
        apply(l, r, {a, b});
    }

    T prod(int l, int r) {
        return seg.prod(l, r).min;
    }

    T all_prod() {
        return seg.all_prod().min;
    }
};
