/* LCA(G, root): 木 G に対する根を root として Lowest Common Ancestor を求める構造体
    query(u,v): u と v の LCA を求める。計算量 O(logn)
    前処理: O(nlogn)時間, O(nlogn)空間
*/
//// 参照: https://algo-logic.info/lca/
//// verify: https://codeforces.com/contest/1380/problem/E
//// 蟻本実装より高速？？

struct LCA {
    vector<vector<int>> parent;  // parent[k][u]:= u の 2^k 先の親
    vector<int> dist;            // root からの距離
    LCA(const vector<vector<int>> &G, int root = 0) { init(G, root); }

    // 初期化
    void init(const vector<vector<int>> &G, int root = 0) {
        int V = G.size();
        int K = 1;
        while ((1 << K) < V) K++;
        parent.assign(K, vector<int>(V, -1));
        dist.assign(V, -1);
        dfs(G, root, -1, 0);
        for (int k = 0; k + 1 < K; k++) {
            for (int v = 0; v < V; v++) {
                if (parent[k][v] < 0) {
                    parent[k + 1][v] = -1;
                } else {
                    parent[k + 1][v] = parent[k][parent[k][v]];
                }
            }
        }
    }

    // 根からの距離と1つ先の頂点を求める
    void dfs(const vector<vector<int>> &G, int v, int p, int d) {
        parent[0][v] = p;
        dist[v] = d;
        for (auto nv : G[v]) {
            if (nv != p) dfs(G, nv, v, d + 1);
        }
    }

    int query(int u, int v) {
        if (dist[u] < dist[v]) swap(u, v);  // u の方が深いとする
        int K = parent.size();
        // LCA までの距離を同じにする
        for (int k = 0; k < K; k++) {
            if ((dist[u] - dist[v]) >> k & 1) {
                u = parent[k][u];
            }
        }
        // 二分探索で LCA を求める
        if (u == v) return u;
        for (int k = K - 1; k >= 0; k--) {
            if (parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }
        return parent[0][u];
    }

    int get_dist(int u, int v){
        int par = query(u, v);
        return dist[u] + dist[v] - 2*dist[par];
    }
};


//////////////////////////
/* 
    LCA(G, root): 木 G に対する根を root として Lowest Common Ancestor を求める構造体
    頂点u, v間の辺の重みの最大値も求められる。
*/

using P = pair<ll, ll>;
struct LCA {
    vector<vector<int>> parent;  // parent[k][u]:= u の 2^k 先の親
    vector<int> dist;            // root からの距離
    vector<vector<ll>> max_cost; // max_cost[k][u] := uの2^k 先までの辺の最大値
    LCA(const vector<vector<P>> &G, int root = 0) { init(G, root); }

    // 初期化
    void init(const vector<vector<P>> &G, int root = 0) {
        int V = G.size();
        int K = 1;
        while ((1 << K) < V) K++;
        //K += 3;
        parent.assign(K, vector<int>(V, -1));
        max_cost.assign(K, vector<ll>(V, 0));
        dist.assign(V, -1);
        dfs(G, root, -1, 0, 0);
        for (int k = 0; k + 1 < K; k++) {
            for (int v = 0; v < V; v++) {
                if (parent[k][v] < 0) {
                    max_cost[k+1][v] = 0;
                    parent[k + 1][v] = -1;
                } else {
                    chmax(max_cost[k + 1][v], max(max_cost[k][v], max_cost[k][parent[k][v]]));
                    parent[k + 1][v] = parent[k][parent[k][v]];
                    //chmax(max_cost[k][parent[k][v]], max_cost[k + 1][v]);
                }
            }
        }
    }

    // 根からの距離と1つ先の頂点を求める
    void dfs(const vector<vector<P>> &G, int v, int p, ll c, int d) {
        parent[0][v] = p;
        max_cost[0][v] = c;
        dist[v] = d;
        for (auto [nv, c] : G[v]) {
            if (nv != p) dfs(G, nv, v, c, d + 1);
        }
    }

    int query(int u, int v) {
        if (dist[u] < dist[v]) swap(u, v);  // u の方が深いとする
        int K = parent.size();
        // LCA までの距離を同じにする
        for (int k = 0; k < K; k++) {
            if ((dist[u] - dist[v]) >> k & 1) {
                u = parent[k][u];
            }
        }
        // 二分探索で LCA を求める
        if (u == v) return u;
        for (int k = K - 1; k >= 0; k--) {
            if (parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }
        return parent[0][u];
    }

    //// return max edge weight between u and v.
    ll get_max_cost(int u, int v){
        int lca = query(u, v);
        ll ret = 0;
        REP(i,2){
            int diff = dist[u] - dist[lca];
            int x = 0;
            int t = u;
            while(diff > 0){
                if(diff % 2) {
                    chmax(ret, max_cost[x][t]);
                    t = parent[x][t];
                }
                x++;
                diff /= 2;
            }
            swap(u, v);
        }
        return ret;
    }
};