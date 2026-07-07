#pragma once

#include <string>
#include <vector>

using namespace std;


//Zアルゴリズム O(N)
//sの最長共通接頭辞を求める。(ABC141E)
//長さ N の文字列 S について、 i = 2, 3, ..., N での S の i 文字目以降から成る文字列と S の最長共通接頭辞の長さを O (N) で求める
//次に求めるiの位置がLとRの間にある場合、すでに求めた結果を再利用できる。
//(Z[i]!=0 で、i+1について求める場合など)
vector<int> z_algorithm(string s){
    int len = s.length();
    vector<int> Z(len);
    int L, R;
    L = 0, R = 0;
    Z[0] = len;

    for(int i = 1; i < len; i++){
        if(i >= R){
            L = R = i;
            while(R < len && s[R-L] == s[R]) R++;
            Z[i] = R - L;
        }
        else if(i + Z[i-L] < R){
            Z[i] = Z[i-L];
        }
        else{
            L = i;
            while(R < len && s[R-L] == s[R]) R++;
            Z[i] = R - L;
        }
    }
    return Z;
}
