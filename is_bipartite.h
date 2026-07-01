#pragma once

//// 二部グラフ判定

vector<int> G[100100];

//// color[v] = {1, -1};
//// c2v[c] = {v1, v2, ...}
bool is_nibu(int v, int c, vector<int>& color, vector<vector<int>>& c2v){
    color[v] = c;
    c2v[(c+1)/2].push_back(v);
    for(auto& next : G[v]){
        bool valid = true;
        if(color[next] == 0){
            valid = is_nibu(next, -c, color, c2v);
        }
        else if(color[next] == c){
            valid = false;
        }
        if(!valid) return false;
    }

    return true;
}
