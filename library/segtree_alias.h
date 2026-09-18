#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/segtree_alias.h

#include <algorithm>
#include <vector>

#include <atcoder/segtree>

#include "rolling_hash.h"

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

// 使い方:
// RollingHashSegtree seg(rolling_hash_segtree_nodes(string("abracadabra")));
// bool same = seg.prod(0, 4) == seg.prod(7, 11);
// seg.set(3, rolling_hash_segtree_leaf('x'));
//
// 文字列・数列の区間ローリングハッシュを保持するノード
// ハッシュ衝突を減らすため MOD1, MOD2 の二重ハッシュを使用する
struct RollingHashSegtreeNode {
    Hash hash;
    int length;

    bool operator==(const RollingHashSegtreeNode& other) const {
        return hash == other.hash && length == other.length;
    }

    bool operator!=(const RollingHashSegtreeNode& other) const {
        return !(*this == other);
    }
};

// Segment Tree の全ノードで同じ base と冪乗表を共有する
const RollingDequeHash<MOD1>& rolling_hash_segtree_context1() {
    static const RollingDequeHash<MOD1> context(B1);
    return context;
}

const RollingDequeHash<MOD2>& rolling_hash_segtree_context2() {
    static const RollingDequeHash<MOD2> context(B2);
    return context;
}

// left の列の後ろに right の列を連結する
RollingHashSegtreeNode rolling_hash_segtree_op(
    RollingHashSegtreeNode left,
    RollingHashSegtreeNode right
) {
    RollingHashValue<MOD1> hash1 = rolling_hash_segtree_context1().concat(
        RollingHashValue<MOD1>(left.hash.h1, left.length),
        RollingHashValue<MOD1>(right.hash.h1, right.length));
    RollingHashValue<MOD2> hash2 = rolling_hash_segtree_context2().concat(
        RollingHashValue<MOD2>(left.hash.h2, left.length),
        RollingHashValue<MOD2>(right.hash.h2, right.length));
    return {Hash(hash1.hash, hash2.hash), hash1.len};
}

RollingHashSegtreeNode rolling_hash_segtree_e() {
    return {Hash(), 0};
}

// 数列の1要素からleafを作る。0との区別が必要なら呼び出し側で値をずらす
RollingHashSegtreeNode rolling_hash_segtree_value(long long value) {
    return {Hash(mod_norm(value, MOD1), mod_norm(value, MOD2)), 1};
}

// 文字を unsigned char + 1 としてleafを作る
RollingHashSegtreeNode rolling_hash_segtree_leaf(char value) {
    return rolling_hash_segtree_value((long long)static_cast<unsigned char>(value) + 1);
}

vector<RollingHashSegtreeNode> rolling_hash_segtree_nodes(const string& values) {
    vector<RollingHashSegtreeNode> nodes;
    nodes.reserve(values.size());
    for (char value : values) {
        nodes.push_back(rolling_hash_segtree_leaf(value));
    }
    return nodes;
}

vector<RollingHashSegtreeNode> rolling_hash_segtree_nodes(const vector<long long>& values) {
    vector<RollingHashSegtreeNode> nodes;
    nodes.reserve(values.size());
    for (long long value : values) {
        nodes.push_back(rolling_hash_segtree_value(value));
    }
    return nodes;
}

using RollingHashSegtree = atcoder::segtree<
    RollingHashSegtreeNode,
    rolling_hash_segtree_op,
    rolling_hash_segtree_e>;
