#pragma once

#include <vector>

template <class T>
struct RangeSumNode {
    T sum;
    int len;
};

template <class T>
RangeSumNode<T> range_sum_op(RangeSumNode<T> a, RangeSumNode<T> b) {
    return {a.sum + b.sum, a.len + b.len};
}

template <class T>
RangeSumNode<T> range_sum_e() {
    return {T{}, 0};
}

template <class T>
std::vector<RangeSumNode<T>> range_sum_nodes(const std::vector<T>& values) {
    std::vector<RangeSumNode<T>> nodes;
    nodes.reserve(values.size());
    for (T value : values) nodes.push_back({value, 1});
    return nodes;
}

template <class T>
std::vector<RangeSumNode<T>> zero_range_sum_nodes(int n) {
    return std::vector<RangeSumNode<T>>(n, {T{}, 1});
}
