#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/segtree_alias/rolling_hash.h

#include <string>
#include <vector>

#include <atcoder/segtree>

#include "../rolling_hash.h"

// 使い方:
// RollingHashSegtree seg(rolling_hash_segtree_nodes(std::string("abracadabra")));
// bool same = seg.prod(0, 4) == seg.prod(7, 11);
// seg.set(3, rolling_hash_segtree_leaf('x'));
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

inline const RollingDequeHash<MOD1>& rolling_hash_segtree_context1() {
    static const RollingDequeHash<MOD1> context(B1);
    return context;
}

inline const RollingDequeHash<MOD2>& rolling_hash_segtree_context2() {
    static const RollingDequeHash<MOD2> context(B2);
    return context;
}

inline RollingHashSegtreeNode rolling_hash_segtree_op(
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

inline RollingHashSegtreeNode rolling_hash_segtree_e() {
    return {Hash(), 0};
}

inline RollingHashSegtreeNode rolling_hash_segtree_value(long long value) {
    return {Hash(mod_norm(value, MOD1), mod_norm(value, MOD2)), 1};
}

inline RollingHashSegtreeNode rolling_hash_segtree_leaf(char value) {
    return rolling_hash_segtree_value(
        static_cast<long long>(static_cast<unsigned char>(value)) + 1);
}

inline std::vector<RollingHashSegtreeNode> rolling_hash_segtree_nodes(
    const std::string& values
) {
    std::vector<RollingHashSegtreeNode> nodes;
    nodes.reserve(values.size());
    for (char value : values) nodes.push_back(rolling_hash_segtree_leaf(value));
    return nodes;
}

inline std::vector<RollingHashSegtreeNode> rolling_hash_segtree_nodes(
    const std::vector<long long>& values
) {
    std::vector<RollingHashSegtreeNode> nodes;
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
