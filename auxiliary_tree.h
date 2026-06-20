/*
    Auxiliary Tree / Virtual Tree

    lca.hの1番目のLCAライブラリを使う

    できること:
    - 指定頂点集合 vs を含む最小部分木を O(|vs| log |vs|) で圧縮構築する
    - vs と、それらの LCA だけからなる木を作る
    - 辺重みとして元の木上の距離を持たせる
    - 指定頂点集合上の木DPに使える

    例:
    - Typical 90 035 Preserve Connectivity
        -> 仮想木の辺重み合計が答え
    - Codeforces 613D Kingdom and its Cities
        -> 仮想木上でDP
*/

struct AuxiliaryTree {
    struct Edge {
        int to;
        int cost;  // 元の木上での距離
    };

    const vector<vector<int>>& G;
    LCA& lca;

    int N;
    vector<int> tin, tout;
    int timer = 0;

    // build(vs) 後に使う
    vector<int> nodes;              // 仮想木に含まれる頂点
    vector<vector<Edge>> tree;      // tree[v] := 仮想木での v の子

    AuxiliaryTree(const vector<vector<int>>& G, LCA& lca, int root = 0)
        : G(G), lca(lca) {
        N = G.size();
        tin.assign(N, 0);
        tout.assign(N, 0);
        tree.assign(N, {});
        dfs(root, -1);
    }

    void dfs(int v, int p) {
        tin[v] = timer++;
        for (int nv : G[v]) {
            if (nv == p) continue;
            dfs(nv, v);
        }
        tout[v] = timer;
    }

    bool is_ancestor(int u, int v) const {
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    }

    void clear() {
        for (int v : nodes) {
            tree[v].clear();
        }
        nodes.clear();
    }

    /*
        vs:
            重要頂点集合

        戻り値:
            仮想木の根

        build後:
            nodes に仮想木の全頂点が入る
            tree[v] に仮想木上の子が入る
    */
    int build(vector<int> vs) {
        clear();

        auto cmp = [&](int a, int b) {
            return tin[a] < tin[b];
        };

        sort(vs.begin(), vs.end(), cmp);
        vs.erase(unique(vs.begin(), vs.end()), vs.end());

        int K = vs.size();

        // Euler Tour 順で隣り合う頂点の LCA を追加
        for (int i = 0; i + 1 < K; i++) {
            vs.push_back(lca.query(vs[i], vs[i + 1]));
        }

        sort(vs.begin(), vs.end(), cmp);
        vs.erase(unique(vs.begin(), vs.end()), vs.end());

        nodes = vs;

        vector<int> st;

        for (int v : vs) {
            while (!st.empty() && !is_ancestor(st.back(), v)) {
                st.pop_back();
            }

            if (!st.empty()) {
                int p = st.back();
                tree[p].push_back({v, lca.get_dist(p, v)});
            }

            st.push_back(v);
        }

        return nodes[0];
    }
};