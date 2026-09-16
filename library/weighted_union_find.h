#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/weighted_union_find.h

#include <cassert>
#include <utility>
#include <vector>

// potential の差を管理する Union-Find
// diff(x, y) = potential(y) - potential(x)
template<class T = long long>
struct WeightedUnionFind {
    WeightedUnionFind() : component_count_(0) {}

    explicit WeightedUnionFind(int n) {
        init(n);
    }

    void init(int n) {
        assert(n >= 0);
        parent_or_size.assign(n, -1);
        diff_to_parent.assign(n, T{});
        component_count_ = n;
    }

    // x が属する集合の代表元を返す
    int leader(int x) {
        assert(0 <= x && x < n());
        if (parent_or_size[x] < 0) return x;

        int parent = parent_or_size[x];
        int root = leader(parent);
        diff_to_parent[x] += diff_to_parent[parent];
        return parent_or_size[x] = root;
    }

    int find(int x) {
        return leader(x);
    }

    // potential(x) - potential(leader(x)) を返す
    T potential(int x) {
        leader(x);
        return diff_to_parent[x];
    }

    T weight(int x) {
        return potential(x);
    }

    bool same(int x, int y) {
        return leader(x) == leader(y);
    }

    // potential(y) - potential(x) を返す
    // x, y が同じ集合に属することを仮定する
    T diff(int x, int y) {
        assert(same(x, y));
        return potential(y) - potential(x);
    }

    // potential(y) - potential(x) = w という制約を追加する
    // 制約が既存情報と整合するなら true、矛盾するなら false
    bool merge(int x, int y, T w) {
        T weight_x = potential(x);
        T weight_y = potential(y);
        int root_x = leader(x);
        int root_y = leader(y);

        if (root_x == root_y) {
            return weight_y - weight_x == w;
        }

        // root_y を root_x の子にするときの
        // potential(root_y) - potential(root_x)
        T root_diff = w + weight_x - weight_y;

        if (-parent_or_size[root_x] < -parent_or_size[root_y]) {
            std::swap(root_x, root_y);
            root_diff = -root_diff;
        }

        parent_or_size[root_x] += parent_or_size[root_y];
        parent_or_size[root_y] = root_x;
        diff_to_parent[root_y] = root_diff;
        component_count_--;
        return true;
    }

    bool unite(int x, int y, T w) {
        return merge(x, y, w);
    }

    // 制約を追加できるかを、集合を変更せずに判定する
    bool can_merge(int x, int y, T w) {
        if (!same(x, y)) return true;
        return diff(x, y) == w;
    }

    int size(int x) {
        return -parent_or_size[leader(x)];
    }

    int n() const {
        return (int)parent_or_size.size();
    }

    int component_count() const {
        return component_count_;
    }

    // 各連結成分の頂点一覧を返す
    std::vector<std::vector<int>> groups() {
        int vertex_count = n();
        std::vector<int> leaders(vertex_count);
        std::vector<int> group_sizes(vertex_count, 0);
        for (int i = 0; i < vertex_count; i++) {
            leaders[i] = leader(i);
            group_sizes[leaders[i]]++;
        }

        std::vector<std::vector<int>> result(vertex_count);
        for (int i = 0; i < vertex_count; i++) {
            result[i].reserve(group_sizes[i]);
        }
        for (int i = 0; i < vertex_count; i++) {
            result[leaders[i]].push_back(i);
        }

        std::vector<std::vector<int>> non_empty_groups;
        non_empty_groups.reserve(component_count_);
        for (std::vector<int>& group : result) {
            if (!group.empty()) non_empty_groups.push_back(std::move(group));
        }
        return non_empty_groups;
    }

private:
    // root なら -集合サイズ、それ以外なら親の頂点番号
    std::vector<int> parent_or_size;
    // potential(x) - potential(parent(x))
    std::vector<T> diff_to_parent;
    int component_count_;
};

template<class T = long long>
using WeightedDSU = WeightedUnionFind<T>;
