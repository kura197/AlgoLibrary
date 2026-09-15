#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/ordered_set.h

#include <cassert>
#include <cstdint>
#include <functional>
#include <iterator>
#include <set>
#include <vector>

// 部分木サイズを持つランダム化 Treap
// Compare が定める順序で要素を管理し、各操作は期待 O(log N)
// Compare 上で同値な値は同じキーとして扱う
template<class T, class Compare, bool allow_duplicates>
struct OrderedTreap {
    struct Node {
        T value;
        std::uint64_t priority;
        int left;
        int right;
        int multiplicity;
        int subtree_size;

        Node(const T& value_, std::uint64_t priority_)
            : value(value_), priority(priority_), left(-1), right(-1),
              multiplicity(1), subtree_size(1) {}
    };

    explicit OrderedTreap(
        const Compare& compare = Compare(),
        std::uint64_t seed = 0x123456789abcdef0ULL
    ) : root(-1), compare(compare), random_state(seed) {}

    // value を追加する。set で既に存在する場合は false
    bool insert(const T& value) {
        bool inserted = false;
        root = insert_impl(root, value, inserted);
        return inserted;
    }

    // value を1つ削除する。存在しない場合は false
    bool erase(const T& value) {
        bool erased = false;
        root = erase_impl(root, value, false, erased);
        return erased;
    }

    // value を全て削除し、削除した個数を返す
    int erase_all(const T& value) {
        int erased = 0;
        root = erase_all_impl(root, value, erased);
        return erased;
    }

    bool contains(const T& value) const {
        return count(value) > 0;
    }

    int count(const T& value) const {
        int node_id = root;
        while (node_id != -1) {
            if (equivalent(value, nodes[node_id].value)) {
                return nodes[node_id].multiplicity;
            }
            if (compare(value, nodes[node_id].value)) {
                node_id = nodes[node_id].left;
            } else {
                node_id = nodes[node_id].right;
            }
        }
        return 0;
    }

    // Compare の順序で value より前にある要素数
    int order_of_key(const T& value) const {
        int result = 0;
        int node_id = root;
        while (node_id != -1) {
            const Node& node = nodes[node_id];
            if (compare(node.value, value)) {
                result += node_size(node.left) + node.multiplicity;
                node_id = node.right;
            } else {
                node_id = node.left;
            }
        }
        return result;
    }

    // Compare の順序で value より後ろにない要素数
    int order_of_upper_key(const T& value) const {
        int result = 0;
        int node_id = root;
        while (node_id != -1) {
            const Node& node = nodes[node_id];
            if (!compare(value, node.value)) {
                result += node_size(node.left) + node.multiplicity;
                node_id = node.right;
            } else {
                node_id = node.left;
            }
        }
        return result;
    }

    // Compare の順序で0-indexedの k 番目の要素
    T kth(int k) const {
        assert(0 <= k && k < size());
        int node_id = root;
        while (node_id != -1) {
            const Node& node = nodes[node_id];
            int left_size = node_size(node.left);
            if (k < left_size) {
                node_id = node.left;
            } else if (k < left_size + node.multiplicity) {
                return node.value;
            } else {
                k -= left_size + node.multiplicity;
                node_id = node.right;
            }
        }
        assert(false);
        return nodes[root].value;
    }

    // lower_bound / upper_bound の順位。該当要素がなければ size() と等しい
    int lower_bound_index(const T& value) const {
        return order_of_key(value);
    }

    int upper_bound_index(const T& value) const {
        return order_of_upper_key(value);
    }

    // lower_bound / upper_bound の値。該当要素が存在することを仮定する
    T lower_bound(const T& value) const {
        int index = lower_bound_index(value);
        assert(index < size());
        return kth(index);
    }

    T upper_bound(const T& value) const {
        int index = upper_bound_index(value);
        assert(index < size());
        return kth(index);
    }

    // Compare の順序で最初・最後の要素
    T min_element() const {
        return kth(0);
    }

    T max_element() const {
        return kth(size() - 1);
    }

    int size() const {
        return node_size(root);
    }

    bool empty() const {
        return root == -1;
    }

    void clear() {
        nodes.clear();
        free_nodes.clear();
        root = -1;
    }

    void reserve(int capacity) {
        assert(capacity >= 0);
        nodes.reserve(capacity);
        free_nodes.reserve(capacity);
    }

private:
    std::vector<Node> nodes;
    std::vector<int> free_nodes;
    int root;
    Compare compare;
    std::uint64_t random_state;

    bool equivalent(const T& left, const T& right) const {
        return !compare(left, right) && !compare(right, left);
    }

    int node_size(int node_id) const {
        return node_id == -1 ? 0 : nodes[node_id].subtree_size;
    }

    void pull(int node_id) {
        nodes[node_id].subtree_size = node_size(nodes[node_id].left)
                                        + nodes[node_id].multiplicity
                                        + node_size(nodes[node_id].right);
    }

    std::uint64_t next_priority() {
        std::uint64_t value = (random_state += 0x9e3779b97f4a7c15ULL);
        value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
        value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;
        return value ^ (value >> 31);
    }

    int make_node(const T& value) {
        std::uint64_t priority = next_priority();
        if (free_nodes.empty()) {
            nodes.push_back(Node(value, priority));
            return (int)nodes.size() - 1;
        }
        int node_id = free_nodes.back();
        free_nodes.pop_back();
        nodes[node_id] = Node(value, priority);
        return node_id;
    }

    int rotate_left(int node_id) {
        int child = nodes[node_id].right;
        nodes[node_id].right = nodes[child].left;
        nodes[child].left = node_id;
        pull(node_id);
        pull(child);
        return child;
    }

    int rotate_right(int node_id) {
        int child = nodes[node_id].left;
        nodes[node_id].left = nodes[child].right;
        nodes[child].right = node_id;
        pull(node_id);
        pull(child);
        return child;
    }

    int insert_impl(int node_id, const T& value, bool& inserted) {
        if (node_id == -1) {
            inserted = true;
            return make_node(value);
        }
        if (equivalent(value, nodes[node_id].value)) {
            if (allow_duplicates) {
                nodes[node_id].multiplicity++;
                pull(node_id);
                inserted = true;
            }
            return node_id;
        }

        if (compare(value, nodes[node_id].value)) {
            int child = insert_impl(nodes[node_id].left, value, inserted);
            nodes[node_id].left = child;
            if (nodes[child].priority > nodes[node_id].priority) {
                node_id = rotate_right(node_id);
            }
        } else {
            int child = insert_impl(nodes[node_id].right, value, inserted);
            nodes[node_id].right = child;
            if (nodes[child].priority > nodes[node_id].priority) {
                node_id = rotate_left(node_id);
            }
        }
        pull(node_id);
        return node_id;
    }

    int merge(int left, int right) {
        if (left == -1) return right;
        if (right == -1) return left;
        if (nodes[left].priority > nodes[right].priority) {
            nodes[left].right = merge(nodes[left].right, right);
            pull(left);
            return left;
        }
        nodes[right].left = merge(left, nodes[right].left);
        pull(right);
        return right;
    }

    int erase_impl(int node_id, const T& value, bool all, bool& erased) {
        if (node_id == -1) return -1;
        if (equivalent(value, nodes[node_id].value)) {
            erased = true;
            if (!all && nodes[node_id].multiplicity > 1) {
                nodes[node_id].multiplicity--;
                pull(node_id);
                return node_id;
            }
            int merged = merge(nodes[node_id].left, nodes[node_id].right);
            free_nodes.push_back(node_id);
            return merged;
        }

        if (compare(value, nodes[node_id].value)) {
            nodes[node_id].left = erase_impl(nodes[node_id].left, value, all, erased);
        } else {
            nodes[node_id].right = erase_impl(nodes[node_id].right, value, all, erased);
        }
        pull(node_id);
        return node_id;
    }

    int erase_all_impl(int node_id, const T& value, int& erased) {
        if (node_id == -1) return -1;
        if (equivalent(value, nodes[node_id].value)) {
            erased = nodes[node_id].multiplicity;
            int merged = merge(nodes[node_id].left, nodes[node_id].right);
            free_nodes.push_back(node_id);
            return merged;
        }

        if (compare(value, nodes[node_id].value)) {
            nodes[node_id].left = erase_all_impl(nodes[node_id].left, value, erased);
        } else {
            nodes[node_id].right = erase_all_impl(nodes[node_id].right, value, erased);
        }
        pull(node_id);
        return node_id;
    }
};

template<class T, class Compare = std::less<T>>
using OrderedSetTreap = OrderedTreap<T, Compare, false>;

template<class T, class Compare = std::less<T>>
using OrderedMultiSetTreap = OrderedTreap<T, Compare, true>;

// Compare の順序で固定された k 番目の要素を管理する（重複可）
// lower に先頭 min(k + 1, size()) 個、upper に残りを保持する
// insert, erase: O(log N), kth: O(1)
template<class T, class Compare = std::less<T>>
struct OrderedSet {
    // k は0-indexed
    explicit OrderedSet(int k = 0, const Compare& compare = Compare())
        : lower(compare), upper(compare), kth_index(k) {
        assert(k >= 0);
    }

    void insert(const T& value) {
        lower.insert(value);
        rebalance();
    }

    // value を1つ削除する。存在しなければ false
    bool erase(const T& value) {
        typename std::multiset<T, Compare>::iterator it = lower.find(value);
        if (it != lower.end()) {
            lower.erase(it);
            rebalance();
            return true;
        }

        it = upper.find(value);
        if (it == upper.end()) return false;
        upper.erase(it);
        rebalance();
        return true;
    }

    // value を全て削除し、削除した個数を返す
    int erase_all(const T& value) {
        int erased = count(value);
        lower.erase(value);
        upper.erase(value);
        rebalance();
        return erased;
    }

    bool contains(const T& value) const {
        return lower.find(value) != lower.end() || upper.find(value) != upper.end();
    }

    // 計算量: O(log N + count(value))
    int count(const T& value) const {
        return (int)lower.count(value) + (int)upper.count(value);
    }

    bool has_kth() const {
        return size() > kth_index;
    }

    // Compare の順序で0-indexedの k 番目の要素: O(1)
    const T& kth() const {
        assert(has_kth());
        return *std::prev(lower.end());
    }

    // Compare の順序で最初・最後の要素: O(1)
    const T& min_element() const {
        assert(!empty());
        return *lower.begin();
    }

    const T& max_element() const {
        assert(!empty());
        if (!upper.empty()) return *std::prev(upper.end());
        return *std::prev(lower.end());
    }

    int k() const {
        return kth_index;
    }

    int size() const {
        return (int)lower.size() + (int)upper.size();
    }

    bool empty() const {
        return lower.empty() && upper.empty();
    }

    void clear() {
        lower.clear();
        upper.clear();
    }

private:
    std::multiset<T, Compare> lower;
    std::multiset<T, Compare> upper;
    int kth_index;

    void rebalance() {
        int total = size();
        int target = total <= kth_index ? total : kth_index + 1;

        while ((int)lower.size() > target) {
            typename std::multiset<T, Compare>::iterator it = std::prev(lower.end());
            upper.insert(*it);
            lower.erase(it);
        }
        while ((int)lower.size() < target) {
            typename std::multiset<T, Compare>::iterator it = upper.begin();
            lower.insert(*it);
            upper.erase(it);
        }
    }
};
