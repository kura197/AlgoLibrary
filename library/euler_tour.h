#pragma once
// https://github.com/kura197/AlgoLibrary
// References:
// - https://info.atcoder.jp/entry/algorithm_lectures/euler_tour_technique
// - https://nyaannyaan.github.io/library/tree/euler-tour.hpp.html

#include <algorithm>
#include <cassert>
#include <limits>
#include <utility>
#include <vector>

#include <atcoder/fenwicktree>
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

// 根付き木の各頂点に、入る時刻 down[v] と出る時刻 up[v] を割り当てる。
// 構築 O(N), 空間 O(N)。入力は連結な木を想定する。
//
// データ構造は size() 要素確保し、用途に応じて次のように値を置く。
//
// 部分木頂点クエリ:
//   down[v] に value[v] を置く。部分木 v は [down[v], up[v])。
//
// 頂点パスクエリ（和など逆元を持つ可換演算）:
//   down[v] に +value[v]、up[v] に -value[v] を置く。
//   node_query(u, v, f) が u-v パスを表す 2 区間を f に渡す。
//
// 辺パスクエリ（和など逆元を持つ可換演算）:
//   root 以外の v について、辺 parent[v]-v の値を down[v] に正、up[v] に負で置く。
//   edge_query(u, v, f) が u-v パスを表す 2 区間を f に渡す。
struct EulerTour {
    int root = -1;
    std::vector<int> parent;
    std::vector<int> depth;
    std::vector<long long> parent_cost;
    std::vector<int> down;
    std::vector<int> up;

    EulerTour() = default;

    explicit EulerTour(const Graph& tree, int root = 0) {
        init(tree, root);
    }

    void init(const Graph& tree, int new_root = 0) {
        const int n = tree.size();
        root = (n == 0 ? -1 : new_root);
        parent.assign(n, -2);
        depth.assign(n, -1);
        parent_cost.assign(n, 0);
        down.assign(n, -1);
        up.assign(n, -1);
        lca_rmq = internal_euler_tour::LcaSegmentTree();
        if (n == 0) return;

        assert(0 <= root && root < n);

        struct Frame {
            int v;
            int next_edge;
        };

        std::vector<Frame> stack;
        stack.reserve(n);
        std::vector<internal_euler_tour::LcaValue> lca_tour;
        lca_tour.reserve(2 * n - 1);

        parent[root] = -1;
        depth[root] = 0;
        enter_vertex(root, lca_tour);
        stack.push_back({root, 0});

        while (!stack.empty()) {
            Frame& frame = stack.back();
            const int v = frame.v;

            if (frame.next_edge < (int)tree[v].size()) {
                const auto [to, cost] = tree[v][frame.next_edge++];
                if (parent[to] != -2) continue;

                parent[to] = v;
                depth[to] = depth[v] + 1;
                parent_cost[to] = cost;
                enter_vertex(to, lca_tour);
                stack.push_back({to, 0});
                continue;
            }

            up[v] = (int)lca_tour.size();
            stack.pop_back();
            if (parent[v] != -1) {
                const int p = parent[v];
                lca_tour.push_back({depth[p], p});
            }
        }

        // 全頂点を訪問し、各辺を往復した頂点列の長さは 2N-1。
        assert((int)lca_tour.size() == 2 * n - 1);
        lca_rmq = internal_euler_tour::LcaSegmentTree(lca_tour);
    }

    // down/up の添字空間のサイズ。root の up[root] も有効な添字なので 2N。
    int size() const {
        return 2 * (int)down.size();
    }

    std::pair<int, int> idx(int v) const {
        assert_vertex(v);
        return {down[v], up[v]};
    }

    std::pair<int, int> subtree_range(int v) const {
        return idx(v);
    }

    // v の部分木の頂点に対応する [down[v], up[v]) を query に渡す。
    template<class F>
    decltype(auto) subtree_query(int v, F&& query) const {
        assert_vertex(v);
        return std::forward<F>(query)(down[v], up[v]);
    }

    template<class F>
    decltype(auto) subtree_node_query(int v, F&& query) const {
        return subtree_query(v, std::forward<F>(query));
    }

    // v の部分木内の辺を子側頂点の down に置く。v へ入る辺は除く。
    template<class F>
    decltype(auto) subtree_edge_query(int v, F&& query) const {
        assert_vertex(v);
        return std::forward<F>(query)(down[v] + 1, up[v]);
    }

    // u-v パス上の頂点を表す 2 区間を query(left, right) に渡す。
    // 1 区間目は LCA を含み、2 区間目は LCA を含まない。
    template<class F>
    void node_query(int u, int v, F&& query) const {
        assert_vertex(u);
        assert_vertex(v);
        const int w = lca(u, v);
        query(down[w], down[u] + 1);
        query(down[w] + 1, down[v] + 1);
    }

    // u-v パス上の辺を表す 2 区間を query(left, right) に渡す。
    // 辺は子側頂点の down/up に正負の値を置く。
    template<class F>
    void edge_query(int u, int v, F&& query) const {
        assert_vertex(u);
        assert_vertex(v);
        const int w = lca(u, v);
        query(down[w] + 1, down[u] + 1);
        query(down[w] + 1, down[v] + 1);
    }

    bool is_ancestor(int ancestor, int v) const {
        assert_vertex(ancestor);
        assert_vertex(v);
        return down[ancestor] <= down[v] && down[v] < up[ancestor];
    }

    // u と v の最小共通祖先を返す。O(log N)。
    int lca(int u, int v) const {
        assert_vertex(u);
        assert_vertex(v);
        int left = down[u];
        int right = down[v];
        if (left > right) std::swap(left, right);
        return lca_rmq.prod(left, right + 1).second;
    }

private:
    internal_euler_tour::LcaSegmentTree lca_rmq;

    void assert_vertex(int v) const {
        assert(0 <= v && v < (int)down.size());
    }

    void enter_vertex(
        int v,
        std::vector<internal_euler_tour::LcaValue>& lca_tour) {
        down[v] = (int)lca_tour.size();
        lca_tour.push_back({depth[v], v});
    }
};

// Euler Tour Technique による動的な木の辺重みパス和。
// 辺は子側の頂点で指定する。v == root に対する add_edge_weight() は使用できない。
//
// EulerTourEdgePathSum path_sum(tree, root);
// path_sum.add_edge_weight(v, w);  // parent[v]-v の辺の重みに w を加える
// path_sum.distance(u, v);         // u-v パス上の辺重みの和を返す
//
// 構築 O(N)、更新 O(log N)、距離クエリ O(log N)、空間 O(N)。
struct EulerTourEdgePathSum {
    EulerTour tour;

    EulerTourEdgePathSum() = default;

    explicit EulerTourEdgePathSum(const Graph& tree, int root = 0) {
        init(tree, root);
    }

    void init(const Graph& tree, int root = 0) {
        tour.init(tree, root);
        fenwick = atcoder::fenwick_tree<long long>(tour.size());

        for (int v = 0; v < tree.size(); ++v) {
            if (v != tour.root) add_at_vertex(v, tour.parent_cost[v]);
        }
    }

    // 頂点 v と parent[v] を結ぶ辺の重みに value を加える。O(log N)。
    void add_edge_weight(int v, long long value) {
        assert(0 <= v && v < (int)tour.parent.size());
        assert(v != tour.root);
        add_at_vertex(v, value);
    }

    // u-v パス上の辺重みの和を返す。O(log N)。
    long long distance(int u, int v) const {
        long long result = 0;
        tour.edge_query(u, v, [&](int left, int right) {
            result += fenwick.sum(left, right);
        });
        return result;
    }

private:
    // ACL の fenwick_tree::sum() は非 const だが、論理的には読み取りのみ。
    mutable atcoder::fenwick_tree<long long> fenwick;

    void add_at_vertex(int v, long long value) {
        const auto [down, up] = tour.idx(v);
        fenwick.add(down, value);
        fenwick.add(up, -value);
    }
};
