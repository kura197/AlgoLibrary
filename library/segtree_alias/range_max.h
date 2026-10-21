#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/segtree_alias/range_max.h

#include <algorithm>
#include <limits>

#include <atcoder/segtree>

template <class T>
T rmaxq_op(T a, T b) {
    return std::max(a, b);
}

template <class T>
T rmaxq_e() {
    return std::numeric_limits<T>::lowest();
}

template <class T>
using RMaxQ = atcoder::segtree<T, rmaxq_op<T>, rmaxq_e<T>>;
