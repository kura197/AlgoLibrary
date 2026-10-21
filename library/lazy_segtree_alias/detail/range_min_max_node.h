#pragma once

#include <algorithm>
#include <limits>
#include <vector>

template <class T>
struct RangeMinMaxNode {
    T min;
    T max;
    int len;
};

template <class T>
RangeMinMaxNode<T> range_min_max_op(
    RangeMinMaxNode<T> left,
    RangeMinMaxNode<T> right
) {
    if (left.len == 0) return right;
    if (right.len == 0) return left;
    return {
        std::min(left.min, right.min),
        std::max(left.max, right.max),
        left.len + right.len,
    };
}

template <class T>
RangeMinMaxNode<T> range_min_max_e() {
    return {
        std::numeric_limits<T>::max(),
        std::numeric_limits<T>::lowest(),
        0,
    };
}

template <class T>
std::vector<RangeMinMaxNode<T>> range_min_max_nodes(
    const std::vector<T>& values
) {
    std::vector<RangeMinMaxNode<T>> nodes;
    nodes.reserve(values.size());
    for (T value : values) nodes.push_back({value, value, 1});
    return nodes;
}

template <class T>
std::vector<RangeMinMaxNode<T>> zero_range_min_max_nodes(int n) {
    return std::vector<RangeMinMaxNode<T>>(n, {T{}, T{}, 1});
}
