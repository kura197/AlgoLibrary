#pragma once

#include <functional>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

#include "graph.h"
#include "trace.h"

struct DijkstraResult {
    std::vector<long long> dist;
    Trace<int> trace;
    std::vector<int> trace_id;

    std::vector<int> get_path(int v) const {
        if (v < 0 || v >= (int)trace_id.size()) return {};
        if (trace_id[v] == -1) return {};
        return trace.get(trace_id[v]);
    }
};

// 非負辺を持つグラフで、始点 src から各頂点への最短距離を計算する. O(E log V)
// trace により、各頂点への最短路における頂点列を復元できる
DijkstraResult dijkstra(const Graph& graph, int src) {
    const long long INF = std::numeric_limits<long long>::max();
    DijkstraResult result;
    result.dist.assign(graph.size(), INF);
    result.trace_id.assign(graph.size(), -1);
    using State = std::pair<long long, int>;

    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    result.dist[src] = 0;
    result.trace_id[src] = (int)result.trace.add(src, -1);
    pq.push({0, src});

    while (!pq.empty()) {
        auto [cost, v] = pq.top();
        pq.pop();

        if (result.dist[v] < cost) continue;

        for (const auto& [to, edge_cost] : graph[v]) {
            long long next_cost = cost + edge_cost;
            if (result.dist[to] <= next_cost) continue;
            result.dist[to] = next_cost;
            result.trace_id[to] = (int)result.trace.add(to, result.trace_id[v]);
            pq.push({next_cost, to});
        }
    }

    return result;
}

// 非負辺を持つグラフで、始点 src から各頂点への最短距離と最短経路数を計算する. O(E log V)
// path_count は mod > 0 のもとで mod で管理する
std::pair<std::vector<long long>, std::vector<long long>>
dijkstra_count_paths(const Graph& graph, int src, long long mod) {
    const long long INF = std::numeric_limits<long long>::max();
    std::vector<long long> dist(graph.size(), INF);
    std::vector<long long> path_count(graph.size(), 0);
    using State = std::pair<long long, int>;

    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    dist[src] = 0;
    path_count[src] = 1 % mod;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [cost, v] = pq.top();
        pq.pop();

        if (dist[v] < cost) continue;

        for (const auto& [to, edge_cost] : graph[v]) {
            long long next_cost = cost + edge_cost;
            if (dist[to] > next_cost) {
                dist[to] = next_cost;
                path_count[to] = path_count[v];
                pq.push({next_cost, to});
            } else if (dist[to] == next_cost) {
                path_count[to] += path_count[v];
                path_count[to] %= mod;
            }
        }
    }

    return {dist, path_count};
}
