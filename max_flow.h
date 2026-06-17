//// 最大流 Dinic法 O(|E||V|^2)
//// 二部マッチング : O(|E||V|^(1/2))
//// 蟻本 p.194
//// https://atcoder.jp/contests/typical90/tasks/typical90_by

const ll INF = 1LL << 60;
const int MAX_V = 200010;

//// {行き先, 容量, 逆辺}
using Edge = tuple<int, ll, int>;
vector<Edge> G[MAX_V];

//// G に辺と逆辺を追加
void add_edge(int from, int to, ll cap){
    G[from].push_back(Edge(to, cap, G[to].size()));
    G[to].push_back(Edge(from, 0, G[from].size()-1));
}

/// sからの最短距離をBFSで計算
void bfs(int s, vector<int>& level){
    queue<int> que;
    level[s] = 0;
    que.push(s);
    while(!que.empty()){
        int v = que.front();
        que.pop();
        for(auto& [to, cap, rev] : G[v]){
            if(cap > 0 && level[to] < 0){
                level[to] = level[v] + 1;
                que.push(to);
            }
        }
    }
}

/// 増加パスをDFSで探索
ll dfs(int v, int t, ll f, vector<int>& iter, vector<int>& level){
    if(v == t) return f;
    for(int &i = iter[v]; i < (int)G[v].size(); i++){
        auto &[to, cap, rev] = G[v][i];
        if(cap > 0LL && level[v] < level[to]){
            auto d = dfs(to, t, min(f, cap), iter, level);
            if(d > 0LL){
                cap -= d;
                get<1>(G[to][rev]) += d;
                return d;
            }
        }
    }
    return 0;
}

//// sからtへの最大流を求める
ll max_flow(int s, int t){
    ll flow = 0;
    while(true){
        vector<int> level(MAX_V, -1); /// sからの距離
        bfs(s, level);
        if(level[t] < 0) return flow;
        vector<int> iter(MAX_V, 0); /// どこまで調べ終わったか
        ll f;
        while((f = dfs(s, t, INF, iter, level)) > 0){
            flow += f;
        }
    }
}


//// 最大流 Ford-Fulkerson法 O(F*|E|) (F := 最大流の流量)
//// 二部マッチング : O(|V||E|)?
//// 蟻本p.190
//// https://atcoder.jp/contests/practice2/tasks/practice2_d

const ll INF = 1LL << 60;
const int MAX_V = 100100;

//// {行き先, 容量, 逆辺}
//// {to, cap, rev}
//// G[v][to] <-> G[to][rev]
using Edge = tuple<int, ll, int>;
vector<Edge> G[MAX_V];

//// G に辺と逆辺を追加
void add_edge(int from, int to, ll cap){
    G[from].push_back(Edge(to, cap, G[to].size()));
    G[to].push_back(Edge(from, 0, G[from].size()-1));
}

//// 増加パスをdfsで探索
ll find_path(int v, int t, ll f, vector<bool>& used){
    if(v == t) return f;
    used[v] = true;
    for(auto& [to, cap, rev] : G[v]){
        if(!used[to] && cap > 0){
            auto d = find_path(to, t, min(f, cap), used);
            if(d > 0){
                cap -= d;
                get<1>(G[to][rev]) += d;
                return d;
            }
        }
    }
    return 0; 
}

//// sからtへの最大流を求める
ll max_flow(int s, int t){
    ll flow = 0;
    while(1){
        vector<bool> used(MAX_V, false);
        auto f = find_path(s, t, INF, used);
        if(f == 0) break;
        flow += f;
    }
    return flow;
}


/// Example
int main(){
    //// Graph:
    //// 0 --> 1 --- |
    //// |     |     |
    //// |     v     v
    //// ----> 2 --> 3
    add_edge(0, 1, 10);
    add_edge(0, 2, 4);

    add_edge(1, 2, 6);

    add_edge(1, 3, 3);
    add_edge(2, 3, 5);

    int flow = max_flow(0, 3);
}

