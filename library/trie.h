#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/trie.h

#include <cassert>
#include <string>
#include <vector>

using namespace std;

// 文字列を管理する Trie 木
// 例: Trie<26, 'a'> trie;
// 各操作の計算量は文字列長を L として O(L)
template<int char_size = 26, int base = 'a'>
struct Trie {
    static_assert(char_size > 0, "Trie: char_size must be positive");

    struct Node {
        // next[c] := 文字 (base + c) で遷移する子のノード番号。存在しなければ -1
        vector<int> next;
        // このノードで終わる文字列の ID
        vector<int> accept;
        // 親からこのノードへ遷移する文字の、base からの差。root は -1
        int c;
        // このノードを通る、現在登録されている文字列の個数
        int common;

        explicit Node(int c_) : next(char_size, -1), c(c_), common(0) {}
    };

    vector<Node> nodes;
    int root;
    int next_word_id;

    Trie() : root(0), next_word_id(0) {
        nodes.push_back(Node(-1));
    }

    // 文字列を word_id とともに登録する
    void insert(const string& word, int word_id) {
        if (!valid(word)) {
            assert(false && "Trie::insert: character is out of range");
            return;
        }

        int node_id = root;
        nodes[node_id].common++;
        for (char ch : word) {
            int c = (int)ch - base;
            int next_id = nodes[node_id].next[c];
            if (next_id == -1) {
                next_id = (int)nodes.size();
                nodes[node_id].next[c] = next_id;
                nodes.push_back(Node(c));
            }
            node_id = next_id;
            nodes[node_id].common++;
        }
        nodes[node_id].accept.push_back(word_id);
        next_word_id++;
    }

    // 文字列を登録し、挿入順を ID として使用する
    void insert(const string& word) {
        insert(word, next_word_id);
    }

    // word に対応するノード番号を返す。存在しなければ -1
    int find(const string& word) const {
        int node_id = root;
        for (char ch : word) {
            int c = (int)ch - base;
            if (c < 0 || c >= char_size) return -1;
            node_id = nodes[node_id].next[c];
            if (node_id == -1) return -1;
        }
        return node_id;
    }

    // 完全一致、または prefix=true なら前方一致で検索する
    bool search(const string& word, bool prefix = false) const {
        int node_id = find(word);
        if (node_id == -1) return false;
        if (prefix) return nodes[node_id].common > 0;
        return !nodes[node_id].accept.empty();
    }

    // prefix を持つ文字列が存在するか
    bool start_with(const string& prefix) const {
        return search(prefix, true);
    }

    // start_with と同じ
    bool starts_with(const string& prefix) const {
        return start_with(prefix);
    }

    // word と完全一致する文字列の登録個数
    int count(const string& word) const {
        int node_id = find(word);
        if (node_id == -1) return 0;
        return (int)nodes[node_id].accept.size();
    }

    // prefix を持つ文字列の登録個数
    int prefix_count(const string& prefix) const {
        int node_id = find(prefix);
        if (node_id == -1) return 0;
        return nodes[node_id].common;
    }

    // word と一致する文字列を1つ削除する。存在しなければ false
    // ノード自体は削除せず、以後の insert で再利用する
    bool erase(const string& word) {
        vector<int> path{root};
        int node_id = root;
        for (char ch : word) {
            int c = (int)ch - base;
            if (c < 0 || c >= char_size) return false;
            node_id = nodes[node_id].next[c];
            if (node_id == -1) return false;
            path.push_back(node_id);
        }
        if (nodes[node_id].accept.empty()) return false;

        nodes[node_id].accept.pop_back();
        for (int id : path) nodes[id].common--;
        return true;
    }

    // query の prefix として登録されている全文字列の ID を返す
    vector<int> common_prefix_search(const string& query) const {
        vector<int> result;
        int node_id = root;
        result.insert(result.end(), nodes[node_id].accept.begin(), nodes[node_id].accept.end());

        for (char ch : query) {
            int c = (int)ch - base;
            if (c < 0 || c >= char_size) break;
            node_id = nodes[node_id].next[c];
            if (node_id == -1 || nodes[node_id].common == 0) break;
            result.insert(result.end(), nodes[node_id].accept.begin(), nodes[node_id].accept.end());
        }
        return result;
    }

    // 現在登録されている文字列の総数
    int count() const {
        return nodes[root].common;
    }

    bool empty() const {
        return count() == 0;
    }

    // Trie 木が確保しているノード数
    int size() const {
        return (int)nodes.size();
    }

    // 全ての文字列とノードを削除する
    void clear() {
        nodes.clear();
        nodes.push_back(Node(-1));
        next_word_id = 0;
    }

private:
    bool valid(const string& word) const {
        for (char ch : word) {
            int c = (int)ch - base;
            if (c < 0 || c >= char_size) return false;
        }
        return true;
    }
};
