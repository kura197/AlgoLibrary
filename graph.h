#pragma once

#include <vector>
#include <utility>

struct Graph {
    using Edge = std::pair<int, long long>;

    std::vector<std::vector<Edge>> adj;

    Graph() = default;
    Graph(int vertex_count) : adj(vertex_count) {}

    void set_vertex_count(int vertex_count) {
        adj.resize(vertex_count);
    }

    int size() const {
        return (int)adj.size();
    }

    void add_directed_edge(int from, int to, long long cost) {
        adj[from].push_back({to, cost});
    }

    void add_directed_edge(int from, int to) {
        add_directed_edge(from, to, 1);
    }

    void add_edge(int u, int v, long long cost) {
        adj[u].push_back({v, cost});
        adj[v].push_back({u, cost});
    }

    void add_edge(int u, int v) {
        adj[u].push_back({v, 1});
        adj[v].push_back({u, 1});
    }

    std::vector<Edge>& operator[](int v) {
        return adj[v];
    }

    const std::vector<Edge>& operator[](int v) const {
        return adj[v];
    }
};
