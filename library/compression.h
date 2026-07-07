#pragma once

#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <vector>

using namespace std;

// 座標圧縮
// 使い方:
// vector<long long> values = {100, 50, 100, 70};
// Compression comp(values);
// int id = comp.compress(70);         // 1
// long long x = comp.decompress(2);   // 100
// vector<int> ids = comp.compressed(); // {2, 0, 2, 1}
//
// 計算量:
// build(values): O(N log N)
// compress(x): 平均 O(1)
// compress_all(values): 平均 O(N)
// decompress(i): O(1)
struct Compression {
    vector<long long> original_values;
    vector<int> compressed_values;
    unordered_map<long long, int> original_to_compressed;

    Compression() = default;

    explicit Compression(const vector<long long>& values) {
        build(values);
    }

    // 与えられた配列をもとに圧縮テーブルを構築する: O(N log N)
    void build(const vector<long long>& values) {
        original_values = values;
        sort(original_values.begin(), original_values.end());
        original_values.erase(unique(original_values.begin(), original_values.end()), original_values.end());

        original_to_compressed.clear();
        for (int i = 0; i < (int)original_values.size(); i++) {
            original_to_compressed[original_values[i]] = i;
        }

        compressed_values = compress_all(values);
    }

    // 異なる値の個数を返す: O(1)
    int size() const {
        return (int)original_values.size();
    }

    // 圧縮対象が空かどうかを返す: O(1)
    bool empty() const {
        return original_values.empty();
    }

    // value が圧縮対象に含まれるかを返す: 平均 O(1)
    bool contains(long long value) const {
        return original_to_compressed.find(value) != original_to_compressed.end();
    }

    // value を圧縮後の index に変換する: 平均 O(1)
    int compress(long long value) const {
        auto it = original_to_compressed.find(value);
        assert(it != original_to_compressed.end());
        return it->second;
    }

    // 配列全体を圧縮する: 平均 O(N)
    vector<int> compress_all(const vector<long long>& values) const {
        vector<int> result(values.size());
        for (int i = 0; i < (int)values.size(); i++) {
            result[i] = compress(values[i]);
        }
        return result;
    }

    // 圧縮後の index を元の値に戻す: O(1)
    long long decompress(int index) const {
        assert(0 <= index && index < (int)original_values.size());
        return original_values[index];
    }

    // index -> original の変換表を返す: O(1)
    const vector<long long>& originals() const {
        return original_values;
    }

    // build 時に与えた配列を圧縮した結果を返す: O(1)
    const vector<int>& compressed() const {
        return compressed_values;
    }
};
