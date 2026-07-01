// トポロジカルソート O(|V| + |E|)
// return {topological sorted vertex array, vertex to topological idx}
// DAG でない場合、topo.size() < V になる。
// topo に含まれない頂点の rank は -1。
pair<vector<int>, vector<int>> Topological_Sort(const vector<vector<int>>& G) {
    const int V = G.size();

    vector<int> indeg(V, 0);
    for (int v = 0; v < V; v++) {
        for (int nv : G[v]) {
            indeg[nv]++;
        }
    }

    queue<int> que;
    for (int v = 0; v < V; v++) {
        if (indeg[v] == 0) que.push(v);
    }

    vector<int> topo;
    vector<int> rank(V, -1);

    while (!que.empty()) {
        int v = que.front();
        que.pop();

        rank[v] = (int)topo.size();
        topo.push_back(v);

        for (int nv : G[v]) {
            indeg[nv]--;
            if (indeg[nv] == 0) {
                que.push(nv);
            }
        }
    }

    return {topo, rank};
}