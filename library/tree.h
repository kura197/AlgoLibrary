#pragma once

#include <algorithm>
#include <queue>
#include <utility>
#include <vector>

#include "graph.h"

using namespace std;

namespace internal_tree {

void dfs_centroids(const Graph& tree,
                   int v,
                   int parent,
                   vector<int>& subtree_size,
                   vector<int>& centroids) {
    int vertex_count = tree.size();
    subtree_size[v] = 1;
    bool is_centroid = true;

    for (const auto& [to, cost] : tree[v]) {
        (void)cost;
        if (to == parent) continue;
        dfs_centroids(tree, to, v, subtree_size, centroids);
        if (subtree_size[to] > vertex_count / 2) is_centroid = false;
        subtree_size[v] += subtree_size[to];
    }

    if (vertex_count - subtree_size[v] > vertex_count / 2) is_centroid = false;
    if (is_centroid) centroids.push_back(v);
}

pair<vector<int>, vector<int>> bfs_tree(const Graph& tree, int start) {
    int vertex_count = tree.size();
    vector<int> dist(vertex_count, -1);
    vector<int> parent(vertex_count, -1);
    queue<int> que;

    dist[start] = 0;
    que.push(start);

    while (!que.empty()) {
        int v = que.front();
        que.pop();
        for (const auto& [to, cost] : tree[v]) {
            (void)cost;
            if (dist[to] != -1) continue;
            dist[to] = dist[v] + 1;
            parent[to] = v;
            que.push(to);
        }
    }

    return {dist, parent};
}

}  // namespace internal_tree

// 木の重心を求める. 非重み木として扱う.
vector<int> get_centroids(const Graph& tree) {
    vector<int> centroids;
    if (tree.size() == 0) return centroids;

    vector<int> subtree_size(tree.size(), 0);
    internal_tree::dfs_centroids(tree, 0, -1, subtree_size, centroids);
    return centroids;
}

// 木の直径と中心を求める. 非重み木として扱う.
// return {diameter, center vertex indices (1 or 2)}
pair<int, vector<int>> get_tree_center(const Graph& tree) {
    if (tree.size() == 0) return {0, {}};

    auto [dist0, parent0] = internal_tree::bfs_tree(tree, 0);
    (void)parent0;
    int v0 = max_element(dist0.begin(), dist0.end()) - dist0.begin();

    auto [dist1, parent1] = internal_tree::bfs_tree(tree, v0);
    int v1 = max_element(dist1.begin(), dist1.end()) - dist1.begin();
    int diameter = dist1[v1];

    vector<int> path;
    for (int v = v1; v != -1; v = parent1[v]) {
        path.push_back(v);
    }

    vector<int> centers;
    if (diameter % 2 == 0) {
        centers.push_back(path[diameter / 2]);
    } else {
        centers.push_back(path[diameter / 2]);
        centers.push_back(path[diameter / 2 + 1]);
    }

    return {diameter, centers};
}
