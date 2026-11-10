#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/functional_graph.h

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <vector>
#include "graph.h"

// 各頂点からちょうど1本の辺が出る有向グラフ。頂点番号は [0, N)。
// 構築・再構築 O(N log(N + 1))、空間 O(N log(N + 1))。再帰は使用しない。
// 構築後に辺や分解結果を直接変更せず、変更時は init() で再構築する。
// 使用例:
//   FunctionalGraph fg({1, 2, 3, 2});  // 0 -> 1 -> 2 -> 3 -> 2
//   fg.jump(0, 5);           // 3
//   fg.distance(0, 3);       // 3
//   fg.distance(3, 0);       // -1
//   fg.reachable_count(0);   // 4 (開始頂点も含む)
//   fg.first_meeting_time(0, 2);  // 2 (両者が同時に1辺ずつ移動)
struct FunctionalGraph {
    std::vector<int> next;
    // cycles[id] は辺の向きに並べたサイクル頂点列。
    // 各弱連結成分にサイクルは1つだけ存在する。
    std::vector<std::vector<int>> cycles;
    // cycle_id[v] := v が最終的に到達するサイクルの番号 (弱連結成分番号)
    std::vector<int> cycle_id;
    // depth[v] := v からサイクルまでの辺数 (サイクル上では0)
    std::vector<int> depth;
    // entry[v] := v から最初に到達するサイクル頂点
    std::vector<int> entry;
    // cycle_pos[v] := サイクル内の添字。サイクル外では -1
    std::vector<int> cycle_pos;
    // component_size[id] := 弱連結成分内の頂点数
    std::vector<int> component_size;

    // 空のグラフを作る。O(1)
    FunctionalGraph() = default;

    // successor[v] が v の行き先。自己ループ・空グラフも扱える。
    // O(N log(N + 1))
    explicit FunctionalGraph(const std::vector<int>& successor) {
        init(successor);
    }

    // 行き先を初期化リストで指定する。O(N log(N + 1))
    // 例: FunctionalGraph fg({0}); は頂点0の自己ループ。
    FunctionalGraph(std::initializer_list<int> successor)
        : FunctionalGraph(std::vector<int>(successor)) {}

    // Graph の有向辺の行き先から構築する。辺重みは無視する。
    // 各頂点の出次数がちょうど1である必要がある。O(N log(N + 1))
    explicit FunctionalGraph(const Graph& graph) {
        std::vector<int> successor(graph.size());
        for (int v = 0; v < graph.size(); ++v) {
            assert(graph[v].size() == 1);
            successor[v] = graph[v][0].first;
        }
        init(successor);
    }

    // 行き先の配列から再構築する。O(N log(N + 1))
    void init(const std::vector<int>& successor) {
        next = successor;
        const int n = size();
        cycles.clear();
        cycle_id.assign(n, -1);
        depth.assign(n, 0);
        entry.assign(n, -1);
        cycle_pos.assign(n, -1);
        component_size.clear();

        std::vector<int> indegree(n, 0);
        for (int to : next) {
            assert(0 <= to && to < n);
            ++indegree[to];
        }

        // 入次数0の頂点を順に除くと、サイクルだけが残る。
        std::vector<int> order;
        order.reserve(n);
        for (int v = 0; v < n; ++v) {
            if (indegree[v] == 0) order.push_back(v);
        }
        for (int i = 0; i < static_cast<int>(order.size()); ++i) {
            const int to = next[order[i]];
            if (--indegree[to] == 0) order.push_back(to);
        }

        for (int v = 0; v < n; ++v) {
            if (indegree[v] == 0 || cycle_id[v] != -1) continue;
            const int id = static_cast<int>(cycles.size());
            cycles.emplace_back();
            int u = v;
            do {
                cycle_id[u] = id;
                entry[u] = u;
                cycle_pos[u] = static_cast<int>(cycles.back().size());
                cycles.back().push_back(u);
                u = next[u];
            } while (u != v);
        }

        // 除いた順序の逆順なら、行き先の分解結果は確定済み。
        for (int i = static_cast<int>(order.size()) - 1; i >= 0; --i) {
            const int v = order[i];
            const int to = next[v];
            cycle_id[v] = cycle_id[to];
            depth[v] = depth[to] + 1;
            entry[v] = entry[to];
        }
        component_size.assign(cycles.size(), 0);
        for (int id : cycle_id) ++component_size[id];

        // サイクル外の移動だけに使うので、O(log N) 段で十分。
        up.clear();
        if (n == 0) return;
        int levels = 1;
        while ((std::uint64_t{1} << levels) < static_cast<std::uint64_t>(n)) {
            ++levels;
        }
        up.assign(levels, std::vector<int>(n));
        up[0] = next;
        for (int k = 1; k < levels; ++k) {
            for (int v = 0; v < n; ++v) {
                up[k][v] = up[k - 1][up[k - 1][v]];
            }
        }
    }

    // 頂点数。O(1)
    int size() const {
        return static_cast<int>(next.size());
    }

    // サイクル数 (= 弱連結成分数)。O(1)
    int cycle_count() const {
        return static_cast<int>(cycles.size());
    }

    // v がサイクル上にあるか。O(1)
    bool is_on_cycle(int v) const {
        check_vertex(v);
        return depth[v] == 0;
    }

    // v が最終的に到達するサイクルの長さ。O(1)
    int cycle_length(int v) const {
        check_vertex(v);
        return static_cast<int>(cycles[cycle_id[v]].size());
    }

    // u, v が同じ弱連結成分に属するか (相互到達可能とは限らない)。O(1)
    bool same_component(int u, int v) const {
        check_vertex(u);
        check_vertex(v);
        return cycle_id[u] == cycle_id[v];
    }

    // v から k 辺移動した頂点。k は 0 から UINT64_MAX まで。O(log(N + 1))
    int jump(int v, std::uint64_t k) const {
        check_vertex(v);
        if (k < static_cast<std::uint64_t>(depth[v])) {
            return advance_tree(v, static_cast<int>(k));
        }
        k -= depth[v];
        const auto& cycle = cycles[cycle_id[v]];
        const auto pos = (cycle_pos[entry[v]] + k % cycle.size()) % cycle.size();
        return cycle[pos];
    }

    // u から v への最小移動回数。到達不能なら -1。u == v なら0。
    // O(log(N + 1))
    long long distance(int u, int v) const {
        if (!same_component(u, v)) return -1;
        if (depth[v] > 0) {
            if (depth[u] < depth[v]) return -1;
            const int steps = depth[u] - depth[v];
            return advance_tree(u, steps) == v ? steps : -1;
        }
        const long long length = cycle_length(v);
        const long long offset =
            (cycle_pos[v] - cycle_pos[entry[u]] + length) % length;
        return depth[u] + offset;
    }

    // u から v に到達できるか。O(log(N + 1))
    bool can_reach(int u, int v) const {
        return distance(u, v) != -1;
    }

    // v から到達できる異なる頂点数 (開始頂点も含む)。
    // 同じ頂点を初めて再訪するまでの移動回数にも等しい。O(1)
    int reachable_count(int v) const {
        check_vertex(v);
        return depth[v] + cycle_length(v);
    }

    // u, v が同時に1辺ずつ移動し、初めて同じ頂点にいる時刻を返す。
    // 時刻0を含み、合流しなければ -1。O(log(N + 1))
    long long first_meeting_time(int u, int v) const {
        if (!same_component(u, v)) return -1;
        if (u == v) return 0;
        const long long length = cycle_length(u);
        const auto phase = [&](int x) {
            return (cycle_pos[entry[x]] - static_cast<long long>(depth[x])
                    % length + length) % length;
        };
        if (phase(u) != phase(v)) return -1;
        if (depth[u] != depth[v]) return std::max(depth[u], depth[v]);

        long long steps = 0;
        for (int k = static_cast<int>(up.size()) - 1; k >= 0; --k) {
            if (up[k][u] == up[k][v]) continue;
            u = up[k][u];
            v = up[k][v];
            steps += 1LL << k;
        }
        return steps + 1;
    }

private:
    std::vector<std::vector<int>> up;

    // 頂点番号の検査。O(1)
    void check_vertex(int v) const {
        assert(0 <= v && v < size());
        (void)v;
    }

    // 0 <= steps <= depth[v] の移動。O(log(N + 1))
    int advance_tree(int v, int steps) const {
        for (int k = 0; steps > 0; ++k, steps >>= 1) {
            if (steps & 1) v = up[k][v];
        }
        return v;
    }
};
