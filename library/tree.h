#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/tree.h

#include <algorithm>
#include <cassert>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

#include "graph.h"

using namespace std;

namespace internal_tree {

pair<vector<int>, vector<int>> root_tree(const Graph& tree) {
    int vertex_count = tree.size();
    if (vertex_count == 0) return {{}, {}};

    vector<int> parent(vertex_count, -2);
    vector<int> order;
    order.reserve(vertex_count);
    parent[0] = -1;
    order.push_back(0);

    for (int i = 0; i < (int)order.size(); i++) {
        int v = order[i];
        for (const auto& [to, cost] : tree[v]) {
            (void)cost;
            if (parent[to] != -2) continue;
            parent[to] = v;
            order.push_back(to);
        }
    }

    assert((int)order.size() == vertex_count);
    return {parent, order};
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

    for (int distance : dist) assert(distance >= 0);
    return {dist, parent};
}

pair<vector<long long>, vector<int>> weighted_tree_distances(
    const Graph& tree,
    int start
) {
    int vertex_count = tree.size();
    vector<long long> dist(vertex_count, -1);
    vector<int> parent(vertex_count, -1);
    queue<int> que;

    dist[start] = 0;
    que.push(start);

    while (!que.empty()) {
        int v = que.front();
        que.pop();
        for (const auto& [to, cost] : tree[v]) {
            assert(cost >= 0);
            if (dist[to] != -1) continue;
            assert(dist[v] <= numeric_limits<long long>::max() - cost);
            dist[to] = dist[v] + cost;
            parent[to] = v;
            que.push(to);
        }
    }

    for (long long distance : dist) assert(distance >= 0);
    return {dist, parent};
}

}  // namespace internal_tree

// 頂点重み付き木の重心を求める.
// 重心 v は、v を除いた各連結成分の頂点重み和が全体の半分以下となる頂点.
// vertex_weights は非負とする.
vector<int> get_vertex_weighted_tree_centroids(
    const Graph& tree,
    const vector<long long>& vertex_weights
) {
    int vertex_count = tree.size();
    assert((int)vertex_weights.size() == vertex_count);
    if (vertex_count == 0) return {};

    auto [parent, order] = internal_tree::root_tree(tree);
    vector<long long> subtree_weight = vertex_weights;
    long long total_weight = 0;
    for (long long weight : vertex_weights) {
        assert(weight >= 0);
        assert(total_weight <= numeric_limits<long long>::max() - weight);
        total_weight += weight;
    }

    for (int i = vertex_count - 1; i > 0; i--) {
        int v = order[i];
        assert(subtree_weight[parent[v]] <=
               numeric_limits<long long>::max() - subtree_weight[v]);
        subtree_weight[parent[v]] += subtree_weight[v];
    }

    vector<int> centroids;
    for (int v = 0; v < vertex_count; v++) {
        long long largest_component = total_weight - subtree_weight[v];
        for (const auto& [to, cost] : tree[v]) {
            (void)cost;
            if (parent[to] == v) {
                largest_component = max(largest_component, subtree_weight[to]);
            }
        }
        if (largest_component <= total_weight - largest_component) {
            centroids.push_back(v);
        }
    }
    return centroids;
}

// 頂点数を重みとした通常の木の重心を求める.
vector<int> get_tree_centroids(const Graph& tree) {
    return get_vertex_weighted_tree_centroids(
        tree,
        vector<long long>(tree.size(), 1));
}

struct TreeCenterResult {
    int diameter;
    vector<int> centers;
    pair<int, int> endpoints;
};

// 木の直径、中心、直径の端点対のひとつを求める. 非重み木として扱う.
// 空の木では {0, {}, {-1, -1}} を返す.
TreeCenterResult get_tree_center(const Graph& tree) {
    if (tree.size() == 0) return {0, {}, {-1, -1}};

    auto [dist0, parent0] = internal_tree::bfs_tree(tree, 0);
    (void)parent0;
    int v0 = (int)(max_element(dist0.begin(), dist0.end()) - dist0.begin());

    auto [dist1, parent1] = internal_tree::bfs_tree(tree, v0);
    int v1 = (int)(max_element(dist1.begin(), dist1.end()) - dist1.begin());
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

    return {diameter, centers, {v0, v1}};
}

struct WeightedTreeCenterResult {
    long long diameter;
    vector<int> centers;
    pair<int, int> endpoints;
};

// 辺重み付き木の直径、頂点中心、直径の端点対のひとつを求める.
// 辺重みは非負とする.
// centers は、各頂点への最大重み付き距離を最小にする頂点を全て含む.
// 辺の内部にある幾何的な中心点ではなく、頂点に限定した中心を返す.
WeightedTreeCenterResult get_weighted_tree_center(const Graph& tree) {
    if (tree.size() == 0) return {0, {}, {-1, -1}};

    auto [dist0, parent0] = internal_tree::weighted_tree_distances(tree, 0);
    (void)parent0;
    int v0 = (int)(max_element(dist0.begin(), dist0.end()) - dist0.begin());

    auto [dist_from_v0, parent1] =
        internal_tree::weighted_tree_distances(tree, v0);
    (void)parent1;
    int v1 = (int)(max_element(dist_from_v0.begin(), dist_from_v0.end()) -
                   dist_from_v0.begin());
    long long diameter = dist_from_v0[v1];

    auto [dist_from_v1, parent2] =
        internal_tree::weighted_tree_distances(tree, v1);
    (void)parent2;

    long long radius = numeric_limits<long long>::max();
    vector<int> centers;
    for (int v = 0; v < tree.size(); v++) {
        long long eccentricity = max(dist_from_v0[v], dist_from_v1[v]);
        if (eccentricity < radius) {
            radius = eccentricity;
            centers = {v};
        } else if (eccentricity == radius) {
            centers.push_back(v);
        }
    }

    return {diameter, centers, {v0, v1}};
}
