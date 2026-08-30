#pragma once
// https://github.com/kura197/AlgoLibrary
// Reference: https://info.atcoder.jp/entry/algorithm_lectures/euler_tour_technique

#include <algorithm>
#include <cassert>
#include <limits>
#include <utility>
#include <vector>

#include <atcoder/segtree>

#include "graph.h"

namespace internal_euler_tour {

using LcaValue = std::pair<int, int>;  // {depth, vertex}

inline LcaValue lca_op(LcaValue a, LcaValue b) {
    return std::min(a, b);
}

inline LcaValue lca_e() {
    return {std::numeric_limits<int>::max(), -1};
}

using LcaSegmentTree = atcoder::segtree<LcaValue, lca_op, lca_e>;

}  // namespace internal_euler_tour

// 根付き木を Euler Tour Technique で使う列に変換する。構築 O(N), 空間 O(N)。
//
// 部分木クエリ:
//   vertex_tour の [in[v], out[v]) が v の部分木に一致する。
//
// 根からの辺パスクエリ:
//   親から子 v への辺の値 x を edge_in[v] に +x、edge_out[v] に -x と置く。
//   edge_tour の先頭から edge_in[v] までの総和が root -> v のパスの総和になる。
//
// LCA クエリ:
//   lca(u, v) は walk[first[u] ... first[v]] の深さ最小頂点を RMQ で返す。
struct EulerTour {
    struct DirectedEdge {
        int from;
        int to;
        long long cost;
    };

    int root = -1;
    std::vector<int> parent;
    std::vector<int> depth;

    // 各頂点を初訪問時に一度だけ並べた列 (pre-order)。長さ N。
    std::vector<int> vertex_tour;
    std::vector<int> in;
    std::vector<int> out;

    // 各辺を往路・復路の順に並べた有向辺列。長さ 2(N-1)。
    std::vector<DirectedEdge> edge_tour;
    std::vector<int> edge_in;   // parent[v] -> v の位置。root は -1。
    std::vector<int> edge_out;  // v -> parent[v] の位置。root は -1。

    // DFS 中にいる頂点を、戻りも省略せず並べた列。長さ 2N-1。
    std::vector<int> walk;
    std::vector<int> first;

    EulerTour() = default;

    explicit EulerTour(const Graph& tree, int root = 0) {
        init(tree, root);
    }

    void init(const Graph& tree, int new_root = 0) {
        const int n = tree.size();
        root = (n == 0 ? -1 : new_root);

        parent.assign(n, -2);
        depth.assign(n, -1);
        in.assign(n, -1);
        out.assign(n, -1);
        edge_in.assign(n, -1);
        edge_out.assign(n, -1);
        first.assign(n, -1);
        vertex_tour.clear();
        edge_tour.clear();
        walk.clear();
        lca_rmq = internal_euler_tour::LcaSegmentTree();

        vertex_tour.reserve(n);
        if (n > 0) {
            edge_tour.reserve(2 * (n - 1));
            walk.reserve(2 * n - 1);
        }
        if (n == 0) return;

        assert(0 <= root && root < n);

        struct Frame {
            int v;
            int next_edge;
        };

        std::vector<Frame> stack;
        stack.reserve(n);
        std::vector<long long> parent_cost(n, 0);

        parent[root] = -1;
        depth[root] = 0;
        enter_vertex(root);
        stack.push_back({root, 0});

        while (!stack.empty()) {
            Frame& frame = stack.back();
            const int v = frame.v;

            if (frame.next_edge < (int)tree[v].size()) {
                const auto [to, cost] = tree[v][frame.next_edge++];
                if (parent[to] != -2) continue;

                parent[to] = v;
                parent_cost[to] = cost;
                depth[to] = depth[v] + 1;
                edge_in[to] = (int)edge_tour.size();
                edge_tour.push_back({v, to, cost});
                enter_vertex(to);
                stack.push_back({to, 0});
                continue;
            }

            out[v] = (int)vertex_tour.size();
            stack.pop_back();
            if (parent[v] != -1) {
                edge_out[v] = (int)edge_tour.size();
                edge_tour.push_back({v, parent[v], parent_cost[v]});
                walk.push_back(parent[v]);
            }
        }

        // 入力は連結な木であることを前提とする。
        assert((int)vertex_tour.size() == n);

        std::vector<internal_euler_tour::LcaValue> lca_values;
        lca_values.reserve(walk.size());
        for (int v : walk) {
            lca_values.push_back({depth[v], v});
        }
        lca_rmq = internal_euler_tour::LcaSegmentTree(lca_values);
    }

    std::pair<int, int> subtree_range(int v) const {
        return {in[v], out[v]};
    }

    bool is_ancestor(int ancestor, int v) const {
        return in[ancestor] <= in[v] && out[v] <= out[ancestor];
    }

    // u と v の最小共通祖先を返す。O(log N)。
    int lca(int u, int v) const {
        assert(0 <= u && u < (int)parent.size());
        assert(0 <= v && v < (int)parent.size());

        int left = first[u];
        int right = first[v];
        if (left > right) std::swap(left, right);
        return lca_rmq.prod(left, right + 1).second;
    }

private:
    internal_euler_tour::LcaSegmentTree lca_rmq;

    void enter_vertex(int v) {
        in[v] = (int)vertex_tour.size();
        vertex_tour.push_back(v);
        first[v] = (int)walk.size();
        walk.push_back(v);
    }
};
