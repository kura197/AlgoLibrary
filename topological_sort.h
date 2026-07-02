#pragma once

#include "graph.h"

#include <queue>
#include <utility>
#include <vector>

using namespace std;

// トポロジカルソート O(|V| + |E|)
// return {topological sorted vertex array, vertex to topological idx}
// DAG でない場合、topo.size() < V になる。
// topo に含まれない頂点の rank は -1。
// 有向グラフを想定する。Graph には add_directed_edge() を使うこと。
pair<vector<int>, vector<int>> Topological_Sort(const Graph& graph) {
    const int V = graph.size();

    vector<int> indeg(V, 0);
    for (int v = 0; v < V; v++) {
        for (const auto& [nv, cost] : graph[v]) {
            (void)cost;
            indeg[nv]++;
        }
    }

    queue<int> que;
    for (int v = 0; v < V; v++) {
        if (indeg[v] == 0) que.push(v);
    }

    vector<int> topo;
    vector<int> rank(V, -1);

    while (!que.empty()) {
        int v = que.front();
        que.pop();

        rank[v] = (int)topo.size();
        topo.push_back(v);

        for (const auto& [nv, cost] : graph[v]) {
            (void)cost;
            indeg[nv]--;
            if (indeg[nv] == 0) {
                que.push(nv);
            }
        }
    }

    return {topo, rank};
}
