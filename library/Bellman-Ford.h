#pragma once

#include <vector>
#include <limits>
#include "edge.h"

// 始点srcから各頂点への最短距離を計算する. O(VE)
std::vector<long long> get_dist_bellman_ford(int src, int vertex_count, const std::vector<Edge>& edges) {
    const long long INF = std::numeric_limits<long long>::max();
    std::vector<long long> dist(vertex_count, INF);
    dist[src] = 0;

    for (int step = 0; step < vertex_count; step++) {
        bool update = false;
        for (const Edge& e : edges) {
            if (dist[e.from] == INF) continue;
            if (dist[e.to] <= dist[e.from] + e.cost) continue;
            dist[e.to] = dist[e.from] + e.cost;
            update = true;
        }
        if (!update) break;
    }

    return dist;
}

// グラフ全体に負閉路が存在するかを判定する. O(VE)
bool find_negative_loop(int vertex_count, const std::vector<Edge>& edges) {
    std::vector<long long> dist(vertex_count, 0);

    for (int step = 0; step < vertex_count; step++) {
        bool update = false;
        for (const Edge& e : edges) {
            if (dist[e.to] <= dist[e.from] + e.cost) continue;
            dist[e.to] = dist[e.from] + e.cost;
            update = true;
        }
        if (!update) return false;
        if (step == vertex_count - 1) return true;
    }

    return false;
}
