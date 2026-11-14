#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/segtree_alias/range_min.h

#include <algorithm>
#include <limits>

#include <atcoder/segtree>

template <class T>
T rmq_op(T a, T b) {
    return std::min(a, b);
}

template <class T>
T rmq_e() {
    static_assert(
        std::numeric_limits<T>::is_specialized,
        "RMQ<T>: std::numeric_limits<T> must be specialized"
    );
    return std::numeric_limits<T>::max();
}

template <class T>
using RMQ = atcoder::segtree<T, rmq_op<T>, rmq_e<T>>;
