
/// dijkstra法 
/// O(E*logV)

const ll INF = 1e15;
vector<ll> G[200100];
using P = pair<ll, ll>;

void dijkstra(int s, vector<ll>& d){
    for(auto& x : d) x = INF;
    d[s] = 0;
    priority_queue<P, vector<P>, greater<P> > que;
    que.push(make_pair(0, s));
    while(!que.empty()){
        auto [c, v] = que.top();
        que.pop();
        if(d[v] < c) continue;
        for(auto&& nv : G[v]){
            ll nc = c + 1;
            if(d[nv] > nc){
                d[nv] = nc;
                que.push(make_pair(nc, nv));
            }
        }
    }
}


///  最短経路数も同時に求める
/// 参考: https://drken1215.hatenablog.com/entry/2018/02/09/003200
/// verify: https://codeforces.com/contest/1197/submission/143374077

void dijkstra(int s, vector<ll>& d, vector<ll>& num){
    for(auto& x : d) x = INF;
    for(auto& x : num) x = 0;
    d[s] = 0;
    num[s] = 1;
    priority_queue<P, vector<P>, greater<P> > que;
    que.push(make_pair(0, s));
    while(!que.empty()){
        auto [c, v] = que.top();
        que.pop();
        if(d[v] < c) continue;
        for(auto&& [nv, cost] : G[v]){
            ll nc = c + cost;
            if(d[nv] > nc){
                d[nv] = nc;
                num[nv] = num[v];
                que.push(make_pair(nc, nv));
            } else if(d[nv] == nc){
                num[nv] += num[v];
                num[nv] %= MOD;
            }
        }
    }
}


/////////////////////////
struct edge{int to, cost;};
typedef pair<int, int> P;

int V;
vector<edge> G[MAX_V];
int d[MAX_V];


// O(E*logV)
void dijkstra(int s){
    priority_queue<P, vector<P>, greater<P> > que;
    fill(d, d+V, INF);
    d[s] = 0;
    que.push(P(0,s));

    while(!que.empty()){
        P p = que.top(); que.pop();
        int v = p.second;
        if(d[v] < p.first) continue;
        for(int i = 0; i < G[v].size(); i++){
            edge e = G[v][i];
            if(d[e.to] > d[v] + e.cost){
                d[e.to] = d[v] + e.cost;
                que.push(P(d[e.to], e.to));
            }
        }
    }
}

////////////////////////

const ll INF = LLONG_MAX;
const int MAX_V = 100010;

vector<pair<int, ll> > G[MAX_V];
vector<ll> d;

typedef pair<ll, int> P;

void dijkstra(int s, int V){
    d.resize(V, INF);
    d[s] = 0;
    priority_queue<P, vector<P>, greater<P> > que;
    que.push(make_pair(0, s));
    while(!que.empty()){
        auto p = que.top();
        que.pop();
        ll c = p.first;
        int a = p.second;
        if(d[a] < c) continue;
        for(auto&& p : G[a]){
            int b = p.first;
            ll cc = p.second;
            if(d[b] > c + cc){
                d[b] = c + cc;
                que.push(make_pair(d[b], b));
            }
        }
    }
}


