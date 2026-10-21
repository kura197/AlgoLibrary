#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/segtree_alias/maximum_subarray_sum.h

#include <algorithm>
#include <vector>

#include <atcoder/segtree>

// max_subarray_sum が表す部分配列は1要素以上とする。
template <class T>
struct MaximumSubarraySumNode {
    T sum;
    T max_prefix_sum;
    T max_suffix_sum;
    T max_subarray_sum;
    int length;
};

template <class T>
MaximumSubarraySumNode<T> maximum_subarray_sum_op(
    MaximumSubarraySumNode<T> left,
    MaximumSubarraySumNode<T> right
) {
    if (left.length == 0) return right;
    if (right.length == 0) return left;

    return {
        left.sum + right.sum,
        std::max(left.max_prefix_sum, left.sum + right.max_prefix_sum),
        std::max(right.max_suffix_sum, right.sum + left.max_suffix_sum),
        std::max({left.max_subarray_sum,
                  right.max_subarray_sum,
                  left.max_suffix_sum + right.max_prefix_sum}),
        left.length + right.length,
    };
}

template <class T>
MaximumSubarraySumNode<T> maximum_subarray_sum_e() {
    return {T{}, T{}, T{}, T{}, 0};
}

template <class T>
MaximumSubarraySumNode<T> maximum_subarray_sum_leaf(T value) {
    return {value, value, value, value, 1};
}

template <class T>
std::vector<MaximumSubarraySumNode<T>> maximum_subarray_sum_nodes(
    const std::vector<T>& values
) {
    std::vector<MaximumSubarraySumNode<T>> nodes;
    nodes.reserve(values.size());
    for (T value : values) nodes.push_back(maximum_subarray_sum_leaf(value));
    return nodes;
}

template <class T>
using MaximumSubarraySumSegtree = atcoder::segtree<
    MaximumSubarraySumNode<T>,
    maximum_subarray_sum_op<T>,
    maximum_subarray_sum_e<T>>;
