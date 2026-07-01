#pragma once

#include <algorithm>
#include <vector>

#include <atcoder/dsu>

#include "edge.h"

// Kruskal法で、辺集合 edges から最小全域木のコストを計算する. O(E log E)
long long get_mst(int vertex_count, std::vector<Edge> edges) {
    std::sort(edges.begin(), edges.end(), [](const Edge& lhs, const Edge& rhs) {
        return lhs.cost < rhs.cost;
    });

    atcoder::dsu uf(vertex_count);
    long long total_cost = 0;

    for (const Edge& e : edges) {
        if (uf.same(e.from, e.to)) continue;
        uf.merge(e.from, e.to);
        total_cost += e.cost;
    }

    return total_cost;
}
