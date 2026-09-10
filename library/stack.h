#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/stack.h

#include <algorithm>
#include <deque>
#include <stack>
#include <vector>

using namespace std;

using ll = long long;

// ヒストグラム内の最大長方形の面積を返す O(N)
ll largest_rectangle(const vector<ll>& h){
    const int n = (int)h.size();
    ll ret = 0;
    stack<int> st;

    for(int i = 0; i <= n; i++){
        while(!st.empty() && (i == n || h[st.top()] > h[i])){
            ll height = h[st.top()];
            st.pop();
            ll width = st.empty() ? i : i - st.top() - 1;
            ret = max(ret, height * width);
        }
        if(i < n) st.push(i);
    }

    return ret;
}

// 全ての非空部分配列の最小値の総和を返す O(N)
ll sum_subarray_min(const vector<ll>& a){
    const int n = (int)a.size();
    vector<int> left(n), right(n);
    stack<int> st;

    // left[i]: a[i] より真に小さい、左側で最も近い要素
    for(int i = 0; i < n; i++){
        while(!st.empty() && a[st.top()] >= a[i]) st.pop();
        left[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }

    while(!st.empty()) st.pop();

    // right[i]: a[i] 以下の、右側で最も近い要素
    for(int i = n - 1; i >= 0; i--){
        while(!st.empty() && a[st.top()] > a[i]) st.pop();
        right[i] = st.empty() ? n : st.top();
        st.push(i);
    }

    ll ret = 0;
    for(int i = 0; i < n; i++){
        ret += a[i] * (i - left[i]) * (right[i] - i);
    }
    return ret;
}

// 全ての非空部分配列の最大値の総和を返す O(N)
ll sum_subarray_max(const vector<ll>& a){
    const int n = (int)a.size();
    vector<int> left(n), right(n);
    stack<int> st;

    // left[i]: a[i] より真に大きい、左側で最も近い要素
    for(int i = 0; i < n; i++){
        while(!st.empty() && a[st.top()] <= a[i]) st.pop();
        left[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }

    while(!st.empty()) st.pop();

    // right[i]: a[i] 以上の、右側で最も近い要素
    for(int i = n - 1; i >= 0; i--){
        while(!st.empty() && a[st.top()] < a[i]) st.pop();
        right[i] = st.empty() ? n : st.top();
        st.push(i);
    }

    ll ret = 0;
    for(int i = 0; i < n; i++){
        ret += a[i] * (i - left[i]) * (right[i] - i);
    }
    return ret;
}

// 長さ window_size の各部分配列の最小値を返す O(N)
vector<ll> sliding_window_min(const vector<ll>& a, int window_size){
    const int n = (int)a.size();
    vector<ll> ret;
    if(window_size <= 0 || window_size > n) return ret;

    deque<int> deq;
    for(int i = 0; i < n; i++){
        while(!deq.empty() && deq.front() <= i - window_size){
            deq.pop_front();
        }
        while(!deq.empty() && a[deq.back()] >= a[i]){
            deq.pop_back();
        }
        deq.push_back(i);

        if(i + 1 >= window_size){
            ret.push_back(a[deq.front()]);
        }
    }

    return ret;
}

// 長さ window_size の各部分配列の最大値を返す O(N)
vector<ll> sliding_window_max(const vector<ll>& a, int window_size){
    const int n = (int)a.size();
    vector<ll> ret;
    if(window_size <= 0 || window_size > n) return ret;

    deque<int> deq;
    for(int i = 0; i < n; i++){
        while(!deq.empty() && deq.front() <= i - window_size){
            deq.pop_front();
        }
        while(!deq.empty() && a[deq.back()] <= a[i]){
            deq.pop_back();
        }
        deq.push_back(i);

        if(i + 1 >= window_size){
            ret.push_back(a[deq.front()]);
        }
    }

    return ret;
}
