//// 強連結成分分解 (参考 : https://manabitimes.jp/math/1250)
//// https://atcoder.jp/contests/typical90/tasks/typical90_u
//// O(∣V∣+∣E∣) 

vector<ll> G[101000];

using P = pair<ll, ll>;

//// それ以上進めなくなる頂点を順に探索
void dfs1(ll v, ll& cnt, vector<P>& T, vector<bool>& isvisit){
    isvisit[v] = true;
    for(auto& nv : G[v]){
        if(isvisit[nv]) continue;
        dfs1(nv, cnt, T, isvisit);
    }

    T[v] = P(cnt++, v);
}

//// 探索した頂点をgroupに入れる
void dfs2(ll v, vector<ll>& group, vector<bool>& isvisit, vector<vector<ll>>& RG){
    isvisit[v] = true;
    group.push_back(v);
    for(auto& nv : RG[v]){
        if(isvisit[nv]) continue;
        dfs2(nv, group, isvisit, RG);
    }
}

//// 強連結成分分解
void calc_SCC(ll N, vector<vector<ll>>& SCC){
    //// 進めなくなった頂点番号
    //// {順番, 頂点番号}
    vector<P> T(N);

    //// Tを計算
    vector<bool> isvisit1(N);
    ll cnt = 0;
    REP(v,N){
        if(isvisit1[v]) continue;
        dfs1(v, cnt, T, isvisit1);
    }

    //// 辺を逆向きに張ったグラフを用意
    vector<vector<ll>> RG(N);
    REP(v,N){
        for(auto& nv : G[v])
            RG[nv].push_back(v);
    }

    //// 降順に頂点を探索し、すすめる頂点までを同じ成分とする
    sort(T.rbegin(), T.rend());
    vector<bool> isvisit2(N);
    for(auto& [_, v] : T){
        if(isvisit2[v]) continue;
        vector<ll> group;
        dfs2(v, group, isvisit2, RG);
        SCC.push_back(group);
    }
}


//// in main
vector<vector<ll>> SCC;
calc_SCC(N, SCC);
