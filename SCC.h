#include <algorithm>
#include <vector>
#include "graph.h"

// 強連結成分分解 O(V + E)
// Graphの辺の向きをそのまま使い、辺重みは無視する
struct SCC {
    int vertex_count;
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> reverse_graph;
    // component_id[v] := 頂点vが属する成分番号
    // 縮約DAG上で u -> v なら component_id[u] <= component_id[v] になる
    std::vector<int> component_id;

    // Graphから強連結成分分解用の有向グラフを作る O(V + E)
    SCC(const Graph& G)
        : vertex_count(G.size()),
          graph(vertex_count),
          reverse_graph(vertex_count),
          component_id(vertex_count, -1) {
        for (int from = 0; from < vertex_count; from++) {
            for (const auto& [to, cost] : G[from]) {
                graph[from].push_back(to);
                reverse_graph[to].push_back(from);
            }
        }
    }

    // 強連結成分分解を行い、成分の頂点リストをトポロジカル順に返す O(V + E)
    std::vector<std::vector<int>> decompose() {
        std::vector<int> order;
        std::vector<bool> used(vertex_count, false);
        for (int v = 0; v < vertex_count; v++) {
            if (!used[v]) dfs_order(v, used, order);
        }

        std::reverse(order.begin(), order.end());
        std::fill(component_id.begin(), component_id.end(), -1);

        std::vector<std::vector<int>> components;
        for (int v : order) {
            if (component_id[v] != -1) continue;
            components.push_back({});
            int id = (int)components.size() - 1;
            dfs_component(v, id, components.back());
        }

        return components;
    }

private:
    void dfs_order(int v, std::vector<bool>& used, std::vector<int>& order) {
        used[v] = true;
        for (int nv : graph[v]) {
            if (!used[nv]) dfs_order(nv, used, order);
        }
        order.push_back(v);
    }

    void dfs_component(int v, int id, std::vector<int>& component) {
        component_id[v] = id;
        component.push_back(v);
        for (int nv : reverse_graph[v]) {
            if (component_id[nv] == -1) dfs_component(nv, id, component);
        }
    }
};
