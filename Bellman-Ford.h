#include <cstring>

const ll INF = LLONG_MAX;
const ll MAX_V = 100010;

int V,E;
struct edge{int from; int to; ll cost;};
edge es[MAX_V*MAX_V];
ll d[MAX_V];

//sからの最短経路  O(EV)
void shortest_path(int s){
    for(int i = 0; i < V; i++) d[i] = INF;
    d[s] = 0;
    while(true){
        bool update = false;
        for(int i = 0; i < E; i++){
            edge e = es[i];
            if(d[e.from] != INF && d[e.to] > d[e.from] + e.cost){
                d[e.to] = d[e.from] + e.cost;
                update = true;
            }
        }
        if(!update) break;
    }
}

bool find_negative_loop(){
    memset(d, 0, sizeof(d));
    for(int i = 0; i < V; i++){
        for(int j = 0; j < E; j++){
            edge e = es[j];
            if(d[e.to] > d[e.from] + e.cost){
                d[e.to] = d[e.from] + e.cost;
                if(i == V-1) return true;
            }
        }
    }
    return false;
}


//////////////////////////////////

const ll INF = LLONG_MAX;

vector d;
vector<tuple<int, int, ll> > edge;

bool shortest_path(int s, int V, int E){
    bool negative = false;
    fill(d.begin(), d.end(), INF);
    d[s] = 0;
    for(int v = 0; v < V; v++){
        bool update = false;
        for(auto&& e : edge){
            int a = get<0>(e);
            int b = get<1>(e);
            ll c = get<2>(e);
            if(d[a] != INF && d[b] > d[a] + c){
                d[b] = d[a] + c;
                update = true;
            }
        }

        if(v == V-1 && update)
            negative = true;
    }

    return negative;
}
