/*
    LCA(G, root): 木 G に対する根を root として Lowest Common Ancestor を求める構造体
    query(u, v): u と v の LCA を求める。計算量 O(log N)
    get_dist(u, v): u と v の辺数距離を求める。計算量 O(log N)
    get_cost_dist(u, v): u と v の重み付き距離を求める。計算量 O(log N)
    get_max_cost(u, v): u と v の経路上の辺重み最大値を求める。計算量 O(log N)
    前処理: O(N log N)
*/

#include "graph.h"

struct LCA {
    std::vector<std::vector<int>> parent;
    std::vector<int> depth;
    std::vector<long long> cost_dist;
    std::vector<std::vector<long long>> max_cost;

    LCA() = default;

    // LCAを前計算する O(N log N)
    LCA(const Graph& G, int root = 0) { init(G, root); }

    // LCAを前計算する O(N log N)
    void init(const Graph& G, int root = 0) {
        int vertex_count = G.size();
        int log = 1;
        while ((1 << log) < vertex_count) log++;

        parent.assign(log, std::vector<int>(vertex_count, -1));
        depth.assign(vertex_count, -1);
        cost_dist.assign(vertex_count, 0);
        max_cost.assign(log, std::vector<long long>(vertex_count, 0));

        dfs_weighted(G, root, -1, 0, 0, 0);
        build();
    }

    // u と v の LCA を求める O(log N)
    int query(int u, int v) const {
        if (depth[u] < depth[v]) std::swap(u, v);
        int log = (int)parent.size();

        int diff = depth[u] - depth[v];
        for (int k = 0; k < log; k++) {
            if ((diff >> k) & 1) u = parent[k][u];
        }

        if (u == v) return u;
        for (int k = log - 1; k >= 0; k--) {
            if (parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }
        return parent[0][u];
    }

    // u と v の辺数距離を求める O(log N)
    int get_dist(int u, int v) const {
        int lca = query(u, v);
        return depth[u] + depth[v] - 2 * depth[lca];
    }

    // u と v の重み付き距離を求める O(log N)
    long long get_cost_dist(int u, int v) const {
        int lca = query(u, v);
        return cost_dist[u] + cost_dist[v] - 2 * cost_dist[lca];
    }

    // u と v の経路上の辺重み最大値を求める O(log N)
    long long get_max_cost(int u, int v) const {
        int lca = query(u, v);
        long long ret = 0;
        ret = std::max(ret, climb_max_cost(u, depth[u] - depth[lca]));
        ret = std::max(ret, climb_max_cost(v, depth[v] - depth[lca]));
        return ret;
    }

private:
    void dfs_weighted(const Graph& G, int v, int p, long long cost, long long current_cost_dist, int d) {
        parent[0][v] = p;
        max_cost[0][v] = cost;
        cost_dist[v] = current_cost_dist;
        depth[v] = d;
        for (const auto& [nv, edge_cost] : G[v]) {
            if (nv == p) continue;
            dfs_weighted(G, nv, v, edge_cost, current_cost_dist + edge_cost, d + 1);
        }
    }

    void build() {
        int log = (int)parent.size();
        int vertex_count = (int)depth.size();
        for (int k = 0; k + 1 < log; k++) {
            for (int v = 0; v < vertex_count; v++) {
                if (parent[k][v] < 0) {
                    parent[k + 1][v] = -1;
                    max_cost[k + 1][v] = max_cost[k][v];
                } else {
                    parent[k + 1][v] = parent[k][parent[k][v]];
                    max_cost[k + 1][v] = std::max(max_cost[k][v], max_cost[k][parent[k][v]]);
                }
            }
        }
    }

    long long climb_max_cost(int v, int diff) const {
        long long ret = 0;
        int log = (int)parent.size();
        for (int k = 0; k < log; k++) {
            if ((diff >> k) & 1) {
                ret = std::max(ret, max_cost[k][v]);
                v = parent[k][v];
            }
        }
        return ret;
    }
};
