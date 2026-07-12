#pragma once

#include <vector>

#include "graph.h"

using namespace std;

// Rerooting DP
//
// 使い方:
// long long rerooting_merge(long long a, long long b) { return max(a, b); }
// long long rerooting_add_vertex(long long x, int v) { return x; }
// long long rerooting_add_edge(long long x, int from, int to, long long cost) {
//     return x + cost;
// }
// long long rerooting_e() { return 0; }
// Rerooting<
//     long long,
//     rerooting_merge,
//     rerooting_add_vertex,
//     rerooting_add_edge,
//     rerooting_e
// > rerooting(tree);
// vector<long long> ans = rerooting.solve();
//
// 計算量: O(N)
template<
    class DP,
    DP (*merge)(DP, DP),
    DP (*add_vertex)(DP, int),
    DP (*add_edge)(DP, int, int, long long),
    DP (*e)()
>
struct Rerooting {
    const Graph& tree;

    explicit Rerooting(const Graph& tree) : tree(tree) {}

    vector<DP> solve() const {
        int n = tree.size();
        if (n == 0) return {};

        vector<int> parent(n, -1), order;
        order.reserve(n);

        parent[0] = -2;
        order.push_back(0);

        for (int i = 0; i < (int)order.size(); i++) {
            int v = order[i];
            for (const auto& [to, cost] : tree[v]) {
                (void)cost;
                if (parent[to] != -1) continue;
                parent[to] = v;
                order.push_back(to);
            }
        }

        vector<DP> down(n, e()), up(n, e()), ans(n, e());

        for (int i = n - 1; i >= 0; i--) {
            int v = order[i];
            DP acc = e();

            for (const auto& [to, cost] : tree[v]) {
                if (to == parent[v]) continue;
                acc = merge(acc, add_edge(down[to], to, v, cost));
            }

            down[v] = add_vertex(acc, v);
        }

        for (int v : order) {
            int deg = tree[v].size();
            vector<DP> val(deg);

            for (int i = 0; i < deg; i++) {
                auto [to, cost] = tree[v][i];
                if (to == parent[v]) {
                    val[i] = up[v];
                } else {
                    val[i] = add_edge(down[to], to, v, cost);
                }
            }

            vector<DP> pref(deg + 1, e()), suff(deg + 1, e());
            for (int i = 0; i < deg; i++) {
                pref[i + 1] = merge(pref[i], val[i]);
            }
            for (int i = deg - 1; i >= 0; i--) {
                suff[i] = merge(val[i], suff[i + 1]);
            }

            ans[v] = add_vertex(pref[deg], v);

            for (int i = 0; i < deg; i++) {
                auto [to, cost] = tree[v][i];
                if (to == parent[v]) continue;

                DP without_child = merge(pref[i], suff[i + 1]);
                DP from_v = add_vertex(without_child, v);
                up[to] = add_edge(from_v, v, to, cost);
            }
        }

        return ans;
    }
};
