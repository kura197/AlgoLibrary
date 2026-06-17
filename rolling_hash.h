typedef unsigned long long ull;

const ull B = 1e9+7;

// 蟻p332
// 文字列検索 O(b.length())
// aはbに含まれているか？
bool contain(string a, string b){
    int al = a.length();
    int bl = b.length();
    if(al > bl) return false;

    //Bのal乗を計算
    ull t = 1;
    for(int i = 0; i < al; i++) t *= B;

    //aとbの最初のal文字に関するハッシュ値を計算
    ull ah = 0, bh = 0;
    for(int i = 0; i < al; i++) ah = ah * B + a[i];
    for(int i = 0; i < al; i++) bh = bh * B + b[i];

    //bの場所を1ずつ進めながらハッシュ値をチェック
    for(int i = 0; i + al <= bl; i++){
        if(ah == bh) return true;
        if(i + al < bl) bh = bh * B + b[i+al] - b[i] * t;
    }

    return false;
}


// aの末尾とbの先頭を何文字重ねることができるか
int overlap(string a, string b){
    int al = a.length();
    int bl = b.length();
    int ans = 0;
    ull ah = 0, bh = 0, t = 1;
    for(int i = 1; i <= min(al, bl); i++){
        ah = ah + a[al-i] * t;
        bh = bh * B + b[i-1];
        if(ah == bh) ans = i;
        t *= B;
    }
    return ans;
}




//////////////////////////////////

/// hash function
ull get_hash(const vector<ll>& vec){
    const ull B = 1e9+7;
    ull hash = 0;
    for(auto x : vec) hash = hash*B + x;
    return hash;
}