#pragma once

#include <vector>

#include "graph.h"

using namespace std;

namespace internal_is_bipartite {

bool dfs(const Graph& graph, int v, int color_value, vector<int>& color, vector<vector<int>>& groups) {
    color[v] = color_value;
    groups[(color_value + 1) / 2].push_back(v);

    for (const auto& [to, cost] : graph[v]) {
        (void)cost;
        if (color[to] == 0) {
            if (!dfs(graph, to, -color_value, color, groups)) return false;
        } else if (color[to] == color_value) {
            return false;
        }
    }

    return true;
}

}  // namespace internal_is_bipartite

// color[v] = {1, -1}
// groups[0] と groups[1] に二部グラフの各パートを格納する
bool is_bipartite(const Graph& graph, vector<int>& color, vector<vector<int>>& groups) {
    int vertex_count = graph.size();
    color.assign(vertex_count, 0);
    groups.assign(2, {});

    for (int v = 0; v < vertex_count; v++) {
        if (color[v] != 0) continue;
        if (!internal_is_bipartite::dfs(graph, v, 1, color, groups)) return false;
    }

    return true;
}

bool is_bipartite(const Graph& graph) {
    vector<int> color;
    vector<vector<int>> groups;
    return is_bipartite(graph, color, groups);
}
