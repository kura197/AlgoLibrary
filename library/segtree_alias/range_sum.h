#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/segtree_alias/range_sum.h

#include <atcoder/segtree>

template <class T>
T rsq_op(T a, T b) {
    return a + b;
}

template <class T>
T rsq_e() {
    return T{};
}

template <class T>
using RSQ = atcoder::segtree<T, rsq_op<T>, rsq_e<T>>;
