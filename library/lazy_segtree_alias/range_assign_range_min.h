#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/lazy_segtree_alias/range_assign_range_min.h

#include <vector>

#include <atcoder/lazysegtree>

#include "detail/range_assign.h"
#include "detail/range_min_max_node.h"

template <class T>
RangeMinMaxNode<T> range_assign_range_min_mapping(
    RangeAssignValue<T> f,
    RangeMinMaxNode<T> x
) {
    if (!f.has_value || x.len == 0) return x;
    return {f.value, f.value, x.len};
}

template <class T>
using RangeAssignRangeMinSegtree = atcoder::lazy_segtree<
    RangeMinMaxNode<T>,
    range_min_max_op<T>,
    range_min_max_e<T>,
    RangeAssignValue<T>,
    range_assign_range_min_mapping<T>,
    range_assign_composition<T>,
    range_assign_id<T>>;

template <class T>
struct RangeAssignRangeMin {
    RangeAssignRangeMinSegtree<T> seg;

    RangeAssignRangeMin() = default;

    explicit RangeAssignRangeMin(int n) : seg(zero_range_min_max_nodes<T>(n)) {}

    explicit RangeAssignRangeMin(const std::vector<T>& values)
        : seg(range_min_max_nodes(values)) {}

    void set(int p, T x) {
        seg.set(p, {x, x, 1});
    }

    T get(int p) {
        return seg.get(p).min;
    }

    void apply(int l, int r, T x) {
        seg.apply(l, r, {x, true});
    }

    T prod(int l, int r) {
        return seg.prod(l, r).min;
    }

    T all_prod() {
        return seg.all_prod().min;
    }
};
