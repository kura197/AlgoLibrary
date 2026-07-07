#pragma once

#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <vector>

using namespace std;

struct Compression {
    vector<long long> original_values;
    vector<int> compressed_values;
    unordered_map<long long, int> original_to_compressed;

    Compression() = default;

    explicit Compression(const vector<long long>& values) {
        build(values);
    }

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

    int size() const {
        return (int)original_values.size();
    }

    bool empty() const {
        return original_values.empty();
    }

    bool contains(long long value) const {
        return original_to_compressed.find(value) != original_to_compressed.end();
    }

    int compress(long long value) const {
        auto it = original_to_compressed.find(value);
        assert(it != original_to_compressed.end());
        return it->second;
    }

    vector<int> compress_all(const vector<long long>& values) const {
        vector<int> result(values.size());
        for (int i = 0; i < (int)values.size(); i++) {
            result[i] = compress(values[i]);
        }
        return result;
    }

    long long decompress(int index) const {
        assert(0 <= index && index < (int)original_values.size());
        return original_values[index];
    }

    const vector<long long>& originals() const {
        return original_values;
    }

    const vector<int>& compressed() const {
        return compressed_values;
    }
};
