#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/segtree_alias.h

#include <algorithm>
#include <limits>
#include <vector>

#include <atcoder/segtree>

#include "rolling_hash.h"

using namespace std;

template<class T>
T rmq_op(T a, T b) {
    return min(a, b);
}

template<class T>
T rmq_e() {
    return numeric_limits<T>::max();
}

template<class T>
using RMQ = atcoder::segtree<T, rmq_op<T>, rmq_e<T>>;

template<class T>
T rsq_op(T a, T b) {
    return a + b;
}

template<class T>
T rsq_e() {
    return T{};
}

template<class T>
using RSQ = atcoder::segtree<T, rsq_op<T>, rsq_e<T>>;

template<class T>
T rmaxq_op(T a, T b) {
    return max(a, b);
}

template<class T>
T rmaxq_e() {
    return numeric_limits<T>::lowest();
}

template<class T>
using RMaxQ = atcoder::segtree<T, rmaxq_op<T>, rmaxq_e<T>>;

// 最大部分和を求めるために各区間で保持する値
// max_subarray_sum が表す部分配列は1要素以上とする
template<class T>
struct MaximumSubarraySumNode {
    T sum;
    T max_prefix_sum;
    T max_suffix_sum;
    T max_subarray_sum;
    int length;
};

template<class T>
MaximumSubarraySumNode<T> maximum_subarray_sum_op(
    MaximumSubarraySumNode<T> left,
    MaximumSubarraySumNode<T> right
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

template<class T>
MaximumSubarraySumNode<T> maximum_subarray_sum_e() {
    return {T{}, T{}, T{}, T{}, 0};
}

template<class T>
MaximumSubarraySumNode<T> maximum_subarray_sum_leaf(T value) {
    return {value, value, value, value, 1};
}

template<class T>
vector<MaximumSubarraySumNode<T>> maximum_subarray_sum_nodes(const vector<T>& values) {
    vector<MaximumSubarraySumNode<T>> nodes;
    nodes.reserve(values.size());
    for (T value : values) {
        nodes.push_back(maximum_subarray_sum_leaf(value));
    }
    return nodes;
}

template<class T>
using MaximumSubarraySumSegtree = atcoder::segtree<
    MaximumSubarraySumNode<T>,
    maximum_subarray_sum_op<T>,
    maximum_subarray_sum_e<T>>;

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
