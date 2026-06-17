#include <vector>

class UnionFind{
    public:
        vector<int> par;
        vector<int> rank;

        UnionFind(int n){
            for(int i = 0; i < n; i++){
                par.push_back(i);
                rank.push_back(0);
            }
        }

        int find(int x){
            if(x == par[x])
                return x;
            else
                return par[x] = find(par[x]);
        }

        void unite(int x, int y){
            x = find(x);
            y = find(y);
            if(x == y) return;

            if(rank[x] < rank[y])
                par[x] = y;
            else{
                par[y] = x;
                if(rank[x] == rank[y]) rank[x]++;
            }
        }

        bool same(int x, int y){
            return find(x) == find(y);
        }
};


struct edge { ll u, v, cost; };

bool comp(const edge &e1, const edge &e2){
    return e1.cost < e2.cost;
}

edge es[MAX_E];
int E;
int V;

//最小全域木  O(|E|*log|V|)
int kruskal(){
    sort(es, es+E, comp);
    UnionFind uf = UnionFind(V);
    int res = 0;
    for(int i = 0; i < E; i++){
        edge e = es[i];
        if(!uf.same(e.u, e.v)){
            uf.unite(e.u, e.v);
            res += e.cost;
        }
    }   
    return res;
}
