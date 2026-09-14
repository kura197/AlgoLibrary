#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/binary_trie.h

#include <cassert>
#include <limits>
#include <type_traits>
#include <vector>

// 非負整数を管理する二進 Trie 木（重複可）
// 登録値は bit_width bit の範囲内である必要がある
// 各操作の計算量は O(bit_width)
template<class T = unsigned int, int bit_width = std::numeric_limits<T>::digits>
struct BinaryTrie {
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "BinaryTrie: T must be an unsigned integer type");
    static_assert(!std::is_same<T, bool>::value, "BinaryTrie: bool is not supported");
    static_assert(0 < bit_width && bit_width <= std::numeric_limits<T>::digits,
                  "BinaryTrie: bit_width is out of range");

    struct Node {
        int next[2];
        int common;

        Node() : next{-1, -1}, common(0) {}
    };

    std::vector<Node> nodes;
    int root;
    T lazy_xor;

    BinaryTrie() : root(0), lazy_xor(0) {
        nodes.push_back(Node());
    }

    // value を multiplicity 個追加する
    void insert(T value, int multiplicity = 1) {
        assert(valid_value(value));
        assert(multiplicity > 0);
        if (!valid_value(value) || multiplicity <= 0) return;

        value = (value ^ lazy_xor) & value_mask();
        int node_id = root;
        nodes[node_id].common += multiplicity;
        for (int bit = bit_width - 1; bit >= 0; bit--) {
            int digit = (int)((value >> bit) & T(1));
            int next_id = nodes[node_id].next[digit];
            if (next_id == -1) {
                next_id = (int)nodes.size();
                nodes[node_id].next[digit] = next_id;
                nodes.push_back(Node());
            }
            node_id = next_id;
            nodes[node_id].common += multiplicity;
        }
    }

    // value を1つ削除する。存在しなければ false
    bool erase(T value) {
        return erase_impl(value, false) > 0;
    }

    // value を全て削除し、削除した個数を返す
    int erase_all(T value) {
        return erase_impl(value, true);
    }

    // value の登録個数
    int count(T value) const {
        assert(valid_value(value));
        if (!valid_value(value)) return 0;

        value = (value ^ lazy_xor) & value_mask();
        int node_id = root;
        for (int bit = bit_width - 1; bit >= 0; bit--) {
            int digit = (int)((value >> bit) & T(1));
            node_id = nodes[node_id].next[digit];
            if (node_id == -1 || nodes[node_id].common == 0) return 0;
        }
        return nodes[node_id].common;
    }

    bool contains(T value) const {
        return count(value) > 0;
    }

    // 0-indexedで k 番目に小さい要素を返す
    T kth_element(int k) const {
        return kth_element(k, T(0));
    }

    // (要素 xor xor_value) の順で k 番目となる、元の要素を返す
    T kth_element(int k, T xor_value) const {
        assert(0 <= k && k < size());
        xor_value &= value_mask();
        T raw = kth_raw(k, lazy_xor ^ xor_value);
        return (raw ^ lazy_xor) & value_mask();
    }

    T min_element() const {
        return kth_element(0);
    }

    T max_element() const {
        return kth_element(size() - 1);
    }

    // (要素 xor value) を最小・最大にする元の要素を返す
    T argmin_xor(T value) const {
        return kth_element(0, value);
    }

    T argmax_xor(T value) const {
        return kth_element(size() - 1, value);
    }

    // 登録要素との xor の最小値・最大値を返す
    T min_xor(T value) const {
        return (argmin_xor(value) ^ value) & value_mask();
    }

    T max_xor(T value) const {
        return (argmax_xor(value) ^ value) & value_mask();
    }

    // min_xor, max_xor の別名
    T xor_min(T value) const {
        return min_xor(value);
    }

    T xor_max(T value) const {
        return max_xor(value);
    }

    // value 未満の要素数を返す
    int count_less(T value) const {
        if (value > value_mask()) return size();
        return count_xor_less(T(0), value);
    }

    // value 以下の要素数を返す
    int count_less_equal(T value) const {
        if (value > value_mask()) return size();
        return count_less(value) + count(value);
    }

    // (要素 xor value) < upper_bound を満たす要素数を返す
    int count_xor_less(T value, T upper_bound) const {
        if (upper_bound > value_mask()) return size();

        value &= value_mask();
        T xor_bias = lazy_xor ^ value;
        int result = 0;
        int node_id = root;
        for (int bit = bit_width - 1; bit >= 0 && node_id != -1; bit--) {
            int bias_digit = (int)((xor_bias >> bit) & T(1));
            int bound_digit = (int)((upper_bound >> bit) & T(1));
            if (bound_digit == 1) {
                int smaller = nodes[node_id].next[bias_digit];
                if (smaller != -1) result += nodes[smaller].common;
                node_id = nodes[node_id].next[bias_digit ^ 1];
            } else {
                node_id = nodes[node_id].next[bias_digit];
            }
        }
        return result;
    }

    // 全ての登録要素に value を xor する O(1)
    void apply_xor(T value) {
        lazy_xor ^= value & value_mask();
    }

    int size() const {
        return nodes[root].common;
    }

    bool empty() const {
        return size() == 0;
    }

    int node_count() const {
        return (int)nodes.size();
    }

    void clear() {
        nodes.clear();
        nodes.push_back(Node());
        root = 0;
        lazy_xor = 0;
    }

private:
    static constexpr T value_mask() {
        return std::numeric_limits<T>::max()
               >> (std::numeric_limits<T>::digits - bit_width);
    }

    static constexpr bool valid_value(T value) {
        return (value & (T)~value_mask()) == 0;
    }

    T kth_raw(int k, T xor_bias) const {
        int node_id = root;
        T result = 0;
        for (int bit = bit_width - 1; bit >= 0; bit--) {
            int zero_digit = (int)((xor_bias >> bit) & T(1));
            int zero_id = nodes[node_id].next[zero_digit];
            int zero_count = zero_id == -1 ? 0 : nodes[zero_id].common;
            int digit = zero_digit;
            if (k >= zero_count) {
                k -= zero_count;
                digit ^= 1;
            }
            node_id = nodes[node_id].next[digit];
            if (digit == 1) result |= T(1) << bit;
        }
        return result;
    }

    int erase_impl(T value, bool all) {
        assert(valid_value(value));
        if (!valid_value(value)) return 0;

        value = (value ^ lazy_xor) & value_mask();
        std::vector<int> path{root};
        int node_id = root;
        for (int bit = bit_width - 1; bit >= 0; bit--) {
            int digit = (int)((value >> bit) & T(1));
            node_id = nodes[node_id].next[digit];
            if (node_id == -1 || nodes[node_id].common == 0) return 0;
            path.push_back(node_id);
        }

        int erased = all ? nodes[node_id].common : 1;
        for (int id : path) nodes[id].common -= erased;
        return erased;
    }
};
