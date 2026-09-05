#pragma once
// https://github.com/kura197/AlgoLibrary
// References:
// - https://info.atcoder.jp/entry/algorithm_lectures/heavy_light_decomposition

/*
    HeavyLightDecomposition(tree, root): 木を root を根として重軽分解する

    id[v] / vertex[i]: 頂点 v と HLD 順の添字 i の相互変換
    head[v]: v が属する heavy path の先頭
    subtree(v, edge): v の部分木に対応する半開区間 [l, r)
    path(u, v, edge): u から v へのパスを HLD 順の半開区間に分解する
    path_query(u, v, vertex, f): 可換なパスクエリの各区間を f に渡す
    path_noncommutative_query(u, v, vertex, f): 向き付きの各区間を f に渡す
    subtree_query(v, vertex, f): 部分木の区間を f に渡す
    lca(u, v): u, v の最小共通祖先
    level_ancestor(v, d): v の祖先のうち深さ d の頂点
    kth_ancestor(v, k): v から k 辺だけ根側の頂点
    jump(u, v, k): u-v パス上で u から k 辺進んだ頂点

    edge = false では頂点を、edge = true では「辺を子側の頂点に置く」
    規約で辺を表す。PathSegment::reversed が true の区間は right-1 から
    left の順に処理する。これにより非可換なパスクエリにも利用できる。

    構築 O(N)、各種祖先クエリ O(log N)、パスの区間数 O(log N)
*/

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

#include "graph.h"

struct HeavyLightDecomposition {
    struct PathSegment {
        int left;
        int right;
        bool reversed;
    };

    int root = -1;
    std::vector<int> parent;
    std::vector<int> depth;
    std::vector<long long> parent_cost;
    std::vector<int> subtree_size;
    std::vector<int> heavy_child;
    std::vector<int> head;
    std::vector<int> id;
    std::vector<int> up;
    std::vector<int> vertex;

    HeavyLightDecomposition() = default;

    explicit HeavyLightDecomposition(const Graph& tree, int new_root = 0) {
        init(tree, new_root);
    }

    // 再帰を使わずに構築するため、パス状の大きな木でもスタックを消費しない。
    void init(const Graph& tree, int new_root = 0) {
        const int n = tree.size();
        root = (n == 0 ? -1 : new_root);
        parent.assign(n, -1);
        depth.assign(n, 0);
        parent_cost.assign(n, 0);
        subtree_size.assign(n, 1);
        heavy_child.assign(n, -1);
        head.assign(n, -1);
        id.assign(n, -1);
        up.assign(n, -1);
        vertex.clear();
        vertex.reserve(n);
        if (n == 0) return;

        assert(0 <= root && root < n);

        std::vector<int> order;
        std::vector<bool> visited(n, false);
        order.reserve(n);
        visited[root] = true;
        order.push_back(root);
        for (int i = 0; i < (int)order.size(); i++) {
            const int v = order[i];
            for (const auto& [to, cost] : tree[v]) {
                if (visited[to]) continue;
                visited[to] = true;
                parent[to] = v;
                depth[to] = depth[v] + 1;
                parent_cost[to] = cost;
                order.push_back(to);
            }
        }
        assert((int)order.size() == n);

        for (int i = (int)order.size() - 1; i >= 0; i--) {
            const int v = order[i];
            int largest_size = 0;
            for (const auto& [to, cost] : tree[v]) {
                (void)cost;
                if (parent[to] != v) continue;
                subtree_size[v] += subtree_size[to];
                if (subtree_size[to] > largest_size) {
                    largest_size = subtree_size[to];
                    heavy_child[v] = to;
                }
            }
        }

        // 各 heavy path を一気に下り、light child は後で処理する。
        std::vector<std::pair<int, int>> paths = {{root, root}};
        while (!paths.empty()) {
            const auto [path_root, path_head] = paths.back();
            paths.pop_back();

            for (int v = path_root; v != -1; v = heavy_child[v]) {
                head[v] = path_head;
                id[v] = (int)vertex.size();
                up[v] = id[v] + subtree_size[v];
                vertex.push_back(v);

                for (const auto& [to, cost] : tree[v]) {
                    (void)cost;
                    if (parent[to] == v && to != heavy_child[v]) {
                        paths.push_back({to, to});
                    }
                }
            }
        }
    }

    int size() const { return (int)vertex.size(); }

    // 頂点 v の HLD 順での位置と部分木の終端 [id[v], up[v])。O(1)
    std::pair<int, int> idx(int v) const {
        assert_vertex(v);
        return {id[v], up[v]};
    }

    std::pair<int, int> subtree_range(int v) const {
        return idx(v);
    }

    // anc が v の祖先かを判定する。O(1)
    bool is_ancestor(int anc, int v) const {
        assert_vertex(anc);
        assert_vertex(v);
        return id[anc] <= id[v] && id[v] < up[anc];
    }

    // v の部分木に対応する HLD 順の半開区間を返す。O(1)
    // edge = true の場合、v と parent[v] を結ぶ辺を除く。
    std::pair<int, int> subtree(int v, bool edge = false) const {
        assert_vertex(v);
        return {id[v] + (edge ? 1 : 0), up[v]};
    }

    // Nyaan 版と同じく vertex = true なら頂点、false なら辺を対象にする。
    template<class F>
    decltype(auto) subtree_query(int v, bool vertex_query, F&& query) const {
        const auto [left, right] = subtree(v, !vertex_query);
        return std::forward<F>(query)(left, right);
    }

    // euler_tour.h と共通の、頂点部分木クエリ用インターフェース。
    template<class F>
    decltype(auto) subtree_query(int v, F&& query) const {
        return subtree_query(v, true, std::forward<F>(query));
    }

    template<class F>
    decltype(auto) subtree_node_query(int v, F&& query) const {
        return subtree_query(v, true, std::forward<F>(query));
    }

    template<class F>
    decltype(auto) subtree_edge_query(int v, F&& query) const {
        return subtree_query(v, false, std::forward<F>(query));
    }

    // 深さ d にある v の祖先を返す。存在しない場合は -1。O(log N)
    int level_ancestor(int v, int d) const {
        assert_vertex(v);
        if (d < 0 || depth[v] < d) return -1;
        while (depth[head[v]] > d) v = parent[head[v]];
        return vertex[id[v] - (depth[v] - d)];
    }

    // v から k 辺だけ根側にある頂点を返す。存在しない場合は -1。O(log N)
    int kth_ancestor(int v, int k) const {
        assert_vertex(v);
        if (k < 0 || depth[v] < k) return -1;
        return level_ancestor(v, depth[v] - k);
    }

    // u と v の最小共通祖先を返す。O(log N)
    int lca(int u, int v) const {
        assert_vertex(u);
        assert_vertex(v);
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) std::swap(u, v);
            u = parent[head[u]];
        }
        return depth[u] < depth[v] ? u : v;
    }

    // u と v の辺数距離を返す。O(log N)
    int distance(int u, int v) const {
        const int w = lca(u, v);
        return depth[u] + depth[v] - 2 * depth[w];
    }

    int dist(int u, int v) const {
        return distance(u, v);
    }

    // u-v パス上で u から k 辺進んだ頂点を返す。存在しない場合は -1。
    // O(log N)
    int jump(int u, int v, int k) const {
        if (k < 0) return -1;
        const int w = lca(u, v);
        const int up_length = depth[u] - depth[w];
        const int path_length = up_length + depth[v] - depth[w];
        if (k > path_length) return -1;
        if (k <= up_length) return kth_ancestor(u, k);
        return kth_ancestor(v, path_length - k);
    }

    // u から v への順序でパスを半開区間へ分解する。O(log N)
    // edge = true の場合、パス上の辺を子側の頂点として列挙する。
    std::vector<PathSegment> path(int u, int v, bool edge = false) const {
        assert_vertex(u);
        assert_vertex(v);
        std::vector<PathSegment> from_u, from_v;

        while (head[u] != head[v]) {
            if (depth[head[u]] >= depth[head[v]]) {
                from_u.push_back({id[head[u]], id[u] + 1, true});
                u = parent[head[u]];
            } else {
                from_v.push_back({id[head[v]], id[v] + 1, false});
                v = parent[head[v]];
            }
        }

        if (depth[u] >= depth[v]) {
            const int left = id[v] + (edge ? 1 : 0);
            if (left < id[u] + 1) from_u.push_back({left, id[u] + 1, true});
        } else {
            const int left = id[u] + (edge ? 1 : 0);
            if (left < id[v] + 1) from_v.push_back({left, id[v] + 1, false});
        }

        std::reverse(from_v.begin(), from_v.end());
        from_u.insert(from_u.end(), from_v.begin(), from_v.end());
        return from_u;
    }

    // 可換なパスクエリ。各区間を昇順の [left, right) で query に渡す。
    // vertex_query = false の場合は LCA を除き、辺を子側頂点で表す。
    template<class F>
    void path_query(int u, int v, bool vertex_query, F&& query) const {
        for (const auto& segment : path(u, v, !vertex_query)) {
            query(segment.left, segment.right);
        }
    }

    // 非可換なパスクエリ。u から v への順に向き付き区間を query に渡す。
    // left < right: [left, right) を昇順に処理する。
    // left > right: [right, left) を left-1 から降順に処理する。
    template<class F>
    void path_noncommutative_query(
        int u, int v, bool vertex_query, F&& query) const {
        for (const auto& segment : path(u, v, !vertex_query)) {
            if (segment.reversed) query(segment.right, segment.left);
            else query(segment.left, segment.right);
        }
    }

    // euler_tour.h と共通のパスクエリ用インターフェース。
    template<class F>
    void node_query(int u, int v, F&& query) const {
        path_query(u, v, true, std::forward<F>(query));
    }

    template<class F>
    void edge_query(int u, int v, F&& query) const {
        path_query(u, v, false, std::forward<F>(query));
    }

    template<class F>
    void node_noncommutative_query(int u, int v, F&& query) const {
        path_noncommutative_query(u, v, true, std::forward<F>(query));
    }

    template<class F>
    void edge_noncommutative_query(int u, int v, F&& query) const {
        path_noncommutative_query(u, v, false, std::forward<F>(query));
    }

    // path() の各区間を u から v への順に callback(left, right, reversed)
    // へ渡す。callback 内では [left, right) を処理する。
    template<class F>
    void for_each_path(int u, int v, F callback, bool edge = false) const {
        for (const auto& segment : path(u, v, edge)) {
            callback(segment.left, segment.right, segment.reversed);
        }
    }

private:
    void assert_vertex(int v) const {
        assert(0 <= v && v < size());
    }
};

using HLD = HeavyLightDecomposition;
