//// 木の重心を求める (参考: https://qiita.com/drken/items/4b4c3f1824339b090202)
//// 要確認

void sub_get_centroids(int v, int par, int N, vector<int>& size_subtree, vector<int>& centroids) {
    size_subtree[v] = 1;
    bool is_centroid = true;
    for(auto nv : G[v]){
        if(nv == par) continue;
        sub_get_centroids(nv, v, N, size_subtree, centroids);
        if(size_subtree[nv] > N/2) is_centroid = false;
        size_subtree[v] += size_subtree[nv];
    }
    if(N - size_subtree[v] > N/2) is_centroid = false;
    if(is_centroid) centroids.push_back(v);
}

vector<int> get_centroids(int N){
    vector<int> centroids;
    vector<int> size_subtree(N);
    sub_get_centroids(0, -1, N, size_subtree, centroids);
    return centroids;
}



////////////////////////////
////////////////////////////

/// 木の直径と, 中心を求める.
/// 直径が奇数の場合、中心を挟む2点を返す
/// 参照: https://atcoder.jp/contests/abc221/submissions/28223133

//// return {diameter, center vertex indices (1 or 2)}
pair<int, vector<int>> get_tree_center(int N){
    //// get vertex distances
    auto get_distant_vertex = [&](int v){
        vector<int> dist(N);
        auto dfs = [&](auto&& self, int v, int p, int d) -> void {
            dist[v] = d;
            for(auto& nv : G[v]){
                if(nv == p) continue;
                self(self, nv, v, d+1);
            }
        };
        dfs(dfs, v, -1, 0);
        return dist;
    };

    //// dist[v0][v1] = diameter
    auto d0 = get_distant_vertex(0);
    int v0 = max_element(d0.begin(), d0.end()) - d0.begin();
    auto d1 = get_distant_vertex(v0);
    int v1 = max_element(d1.begin(), d1.end()) - d1.begin();
    int diameter = *max_element(d1.begin(), d1.end());

    vector<int> centers;
    unordered_map<int, int> dist_path;
    ///// get distance of vertex on path(v0, v1) 
    auto traverse = [&](auto&& self, int v, int p, int d, int target) -> bool {
        bool path = (v == target);
        for(auto& nv : G[v]){
            if(nv == p) continue;
            chmax(path, self(self, nv, v, d+1, target));
        }
        if(path) dist_path[d] = v;
        return path;
    };
    traverse(traverse, v1, -1, 0, v0);

    if(diameter % 2){
        centers.push_back(dist_path[(diameter-1)/2]);
        centers.push_back(dist_path[(diameter+1)/2]);
    } else {
        centers.push_back(dist_path[diameter/2]);
    }

    return make_pair(diameter, centers);
}