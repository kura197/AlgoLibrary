//トポロジカルソート O(|V| + |E|)

int V, E;
vector<int> G[10010];
vector<int> K;

vector<int> Topological_Sort(){
    queue<int> S;
    vector<int> L;
    REP(i,V){
        if(K[i] == 0)
            S.push(i);
    }

    while(!S.empty()){
        int v = S.front();
        S.pop();
        L.push_back(v);
        for(auto&& next : G[v]){
            K[next] -= 1;
            if(K[next] == 0)
                S.push(next);
        }
    }

    return L;
}

int main(){
    vector<int> L = Topological_Sort();

    if(DAG.length() == V){
        //L is correctly sorted
    }
    else{
        //L is not DAG. impossible to sort.
    }
}
