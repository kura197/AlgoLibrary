#pragma once

/// 最長共通部分列  蟻本 p.56 
/// https://atcoder.jp/contests/dp/tasks/dp_f
/// O(|S||T|)

string get_lcs(string& s, string& t){
    int len_s = s.size();
    int len_t = t.size();

    vector<vector<int>> dp(len_s+1, vector<int>(len_t+1, 0));
    for(int i = 0; i < len_s; i++){
        for(int j = 0; j < len_t; j++){
            if(s[i] == t[j])
                dp[i+1][j+1] = dp[i][j] + 1;
            else
                dp[i+1][j+1] = max(dp[i+1][j], dp[i][j+1]);
        }
    }

    ///int max_len = dp[len_s][len_t];
    ///cout << max_len << endl;
    string ret;
    int i = len_s, j = len_t;
    while(i > 0 || j > 0){
        if(i-1 >= 0 && dp[i-1][j] == dp[i][j])
            i--;
        else if(j-1 >= 0 && dp[i][j-1] == dp[i][j])
            j--;
        else{
            i--, j--;
            ret += s[i];
        }
    }

    reverse(ret.begin(), ret.end());
    return ret;
}
