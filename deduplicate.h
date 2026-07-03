#pragma once

#include <algorithm>
#include <vector>

using namespace std;

// vector をソートしてから重複削除した新しい vector を返す
template<class T>
vector<T> deduplicate(vector<T> values) {
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    return values;
}
