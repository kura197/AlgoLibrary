#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/segtree_alias.h

#include <algorithm>
#include <vector>

#include <atcoder/segtree>

using namespace std;

long long rmq_op(long long a, long long b) {
    return min(a, b);
}

long long rmq_e() {
    return (1LL << 60);
}

using RMQ = atcoder::segtree<long long, rmq_op, rmq_e>;

long long rsq_op(long long a, long long b) {
    return a + b;
}

long long rsq_e() {
    return 0;
}

using RSQ = atcoder::segtree<long long, rsq_op, rsq_e>;

long long rmaxq_op(long long a, long long b) {
    return max(a, b);
}

long long rmaxq_e() {
    return -(1LL << 60);
}

using RMaxQ = atcoder::segtree<long long, rmaxq_op, rmaxq_e>;

// 最大部分和を求めるために各区間で保持する値
// max_subarray_sum が表す部分配列は1要素以上とする
struct MaximumSubarraySumNode {
    long long sum;
    long long max_prefix_sum;
    long long max_suffix_sum;
    long long max_subarray_sum;
    int length;
};

MaximumSubarraySumNode maximum_subarray_sum_op(
    MaximumSubarraySumNode left,
    MaximumSubarraySumNode right
) {
    if (left.length == 0) return right;
    if (right.length == 0) return left;

    return {
        left.sum + right.sum,
        max(left.max_prefix_sum, left.sum + right.max_prefix_sum),
        max(right.max_suffix_sum, right.sum + left.max_suffix_sum),
        max({left.max_subarray_sum,
             right.max_subarray_sum,
             left.max_suffix_sum + right.max_prefix_sum}),
        left.length + right.length,
    };
}

MaximumSubarraySumNode maximum_subarray_sum_e() {
    return {0, 0, 0, 0, 0};
}

MaximumSubarraySumNode maximum_subarray_sum_leaf(long long value) {
    return {value, value, value, value, 1};
}

vector<MaximumSubarraySumNode> maximum_subarray_sum_nodes(const vector<long long>& values) {
    vector<MaximumSubarraySumNode> nodes;
    nodes.reserve(values.size());
    for (long long value : values) {
        nodes.push_back(maximum_subarray_sum_leaf(value));
    }
    return nodes;
}

using MaximumSubarraySumSegtree = atcoder::segtree<
    MaximumSubarraySumNode,
    maximum_subarray_sum_op,
    maximum_subarray_sum_e>;
