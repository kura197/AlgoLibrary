#pragma once

#include <vector>

using namespace std;

class UnionFind{
    public:
        vector<int> par;
        vector<int> rank;
        vector<int> size;

        UnionFind(int n){
            for(int i = 0; i < n; i++){
                par.push_back(i);
                rank.push_back(0);
                size.push_back(1);
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

            if(rank[x] < rank[y]){
                par[x] = y;
                size[y] += size[x];
            }
            else{
                par[y] = x;
                size[x] += size[y];
                if(rank[x] == rank[y]) rank[x]++;
            }
        }

        bool same(int x, int y){
            return find(x) == find(y);
        }

        int get_size(int x){
            return size[find(x)];
        }
};
