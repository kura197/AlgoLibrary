#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/aho_corasick.h

#include <array>
#include <cassert>
#include <cstddef>
#include <string>
#include <vector>

// 複数パターンの文字列検索。重複・重なりのある一致も全て数える。
// L := 登録文字列長、T := 検索文字列長、C := char_size、V := ノード数、
// P := 登録パターン数、K := 返却・通知する一致数。
// 空間 O(VC + P)。各操作の計算量は vector の伸長について償却計算量。
// insert() 後は build() が必要。追加登録後も再び build() すれば検索できる。
// パターンは [base, base + C) の文字だけを使用する。
// 検索対象の範囲外の文字は区切りとして扱い、状態を root に戻す。
// 空パターンは、長さ T の文字列の各境界 (0..T) に計 T+1 回一致する。
// nodes は参照用。遷移や集計結果を直接書き換えないこと。
//
// 使用例: "ushers" 中の "he", "she", "hers" を検索する。
//   AhoCorasick<26, 'a'> ac;
//   int id = ac.insert("he");  // ID は0から挿入順
//   ac.insert("she");
//   ac.insert("hers");
//   ac.build();
//   auto counts = ac.count_occurrences("ushers");  // {1, 1, 1}
//   auto matches = ac.find_all("ushers");
//   // Match{start, end, pattern_id} は半開区間 [start, end) の一致。
//   // 1文字ずつ処理する場合: state = ac.step(state, ch);
//   // ac.match_count(state) で、その位置で終わる一致数を O(1) で取得する。
template<int char_size = 26, int base = 'a'>
struct AhoCorasick {
    static_assert(char_size > 0, "AhoCorasick: char_size must be positive");
    static_assert(0 <= base && base <= 255 && char_size <= 256 - base,
                  "AhoCorasick: alphabet must fit in an unsigned char");

    static constexpr int root = 0;

    struct Node {
        std::array<int, char_size> next;  // Trie の遷移。未定義は -1
        std::array<int, char_size> go;    // failure 遷移を補完した遷移表
        std::vector<int> accept;         // このノードで終わるパターンの ID
        int fail = root;
        int output = -1;                // fail 先にある最も近い終端ノード
        long long count = 0;            // fail 先も含め、この状態で終わる一致数

        // O(C)
        Node() {
            next.fill(-1);
            go.fill(root);
        }
    };

    struct Match {
        std::size_t start;
        std::size_t end;
        int pattern_id;
    };

    std::vector<Node> nodes;

    // 空の辞書を作る。O(C)
    AhoCorasick() : nodes(1) {}

    // 登録して挿入順の ID を返す。同じ文字列も別パターンとして登録する。
    // O(L + 1 + AC)。A は新しく作るノード数。build 済み状態を解除する。
    int insert(const std::string& pattern) {
        // 不正な文字がある場合、途中まで登録してしまうことを防ぐ。
        for (unsigned char ch : pattern) {
            if (code(ch) < 0) {
                assert(false && "AhoCorasick::insert: character is out of range");
                return -1;
            }
        }
        int v = root;
        for (unsigned char ch : pattern) {
            const int c = code(ch);
            int to = nodes[v].next[c];
            if (to == -1) {
                to = size();
                nodes[v].next[c] = to;
                nodes.emplace_back();
            }
            v = to;
        }
        const int id = pattern_count();
        terminal.push_back(v);
        lengths.push_back(pattern.size());
        nodes[v].accept.push_back(id);
        built = false;
        return id;
    }

    // failure link・遷移表・一致数を構築する。O(VC + P)
    // 出力 ID を各ノードへコピーせず、output link で列挙する。
    // 追加登録がなければ、繰り返し呼んでも O(1)。
    void build() {
        if (built) return;
        bfs_order.clear();
        bfs_order.push_back(root);
        nodes[root].fail = root;
        nodes[root].output = -1;
        nodes[root].count = nodes[root].accept.size();
        for (std::size_t i = 0; i < bfs_order.size(); ++i) {
            const int v = bfs_order[i];
            for (int c = 0; c < char_size; ++c) {
                const int to = nodes[v].next[c];
                if (to == -1) {
                    nodes[v].go[c] = v == root ? root : nodes[nodes[v].fail].go[c];
                    continue;
                }
                nodes[v].go[c] = to;
                const int fail = v == root ? root : nodes[nodes[v].fail].go[c];
                nodes[to].fail = fail;
                nodes[to].output = nodes[fail].accept.empty()
                    ? nodes[fail].output : fail;
                nodes[to].count = nodes[to].accept.size() + nodes[fail].count;
                bfs_order.push_back(to);
            }
        }
        built = true;
    }

    // 1文字読んだ後の状態を返す。範囲外の文字なら root。O(1)
    int step(int state, char ch) const {
        check_state(state);
        const int c = code(static_cast<unsigned char>(ch));
        return c < 0 ? root : nodes[state].go[c];
    }

    // 現在位置で終わる一致数 (重複・空パターンも含む)。O(1)
    long long match_count(int state) const {
        check_state(state);
        return nodes[state].count;
    }

    // 現在位置で終わるパターン ID を返す。O(K + 1)
    std::vector<int> match_ids(int state) const {
        check_state(state);
        std::vector<int> result;
        for (int v = state; v != -1; v = nodes[v].output) {
            result.insert(result.end(), nodes[v].accept.begin(), nodes[v].accept.end());
        }
        return result;
    }

    // 全一致を callback(start, end, pattern_id) に通知する。
    // end の昇順。同じ end のときは長いパターンから、同じ文字列は ID 順。
    // O(T + K + 1)。callback の実行時間を除く。半開区間 [start, end)。
    template<class Callback>
    void for_each_match(const std::string& text, Callback callback) const {
        check_state(root);
        int state = root;
        for (std::size_t end = 0;; ++end) {
            for (int v = state; v != -1; v = nodes[v].output) {
                for (int id : nodes[v].accept) {
                    callback(end - lengths[id], end, id);
                }
            }
            if (end == text.size()) break;
            state = step(state, text[end]);
        }
    }

    // 全一致の位置・IDを返す。順序は for_each_match と同じ。
    // 時間 O(T + K + 1)、追加空間 O(K)。
    std::vector<Match> find_all(const std::string& text) const {
        std::vector<Match> result;
        for_each_match(text, [&](std::size_t start, std::size_t end, int id) {
            result.push_back({start, end, id});
        });
        return result;
    }

    // パターンごとの出現回数を ID 順に返す。全一致を列挙しない。
    // 時間 O(T + V + P)、追加空間 O(V + P)。
    std::vector<long long> count_occurrences(const std::string& text) const {
        check_state(root);
        std::vector<long long> visits(size(), 0);
        int state = root;
        visits[root] = 1;  // 空パターンの先頭境界での一致
        for (char ch : text) {
            state = step(state, ch);
            ++visits[state];
        }
        for (int i = static_cast<int>(bfs_order.size()) - 1; i > 0; --i) {
            const int v = bfs_order[i];
            visits[nodes[v].fail] += visits[v];
        }
        std::vector<long long> result(pattern_count());
        for (int id = 0; id < pattern_count(); ++id) result[id] = visits[terminal[id]];
        return result;
    }

    // 全パターンの出現回数の合計。全一致を列挙しない。O(T + 1)
    long long count_matches(const std::string& text) const {
        int state = root;
        long long result = match_count(state);
        for (char ch : text) {
            state = step(state, ch);
            result += match_count(state);
        }
        return result;
    }

    // いずれかのパターンが含まれるか。O(T + 1)
    bool contains(const std::string& text) const {
        int state = root;
        if (match_count(state) != 0) return true;
        for (char ch : text) {
            state = step(state, ch);
            if (match_count(state) != 0) return true;
        }
        return false;
    }

    // ノード数 (root を含む)。O(1)
    int size() const { return static_cast<int>(nodes.size()); }

    // 重複・空文字列も含めた登録パターン数。O(1)
    int pattern_count() const { return static_cast<int>(terminal.size()); }

    // ID に対応するパターンの長さ。O(1)
    std::size_t pattern_length(int id) const {
        assert(0 <= id && id < pattern_count());
        return lengths[id];
    }

    // 現在の辞書について build 済みか。O(1)
    bool is_built() const { return built; }

    // 全登録・構築結果を削除し、IDを0から振り直す。O(VC + P)
    void clear() {
        nodes.clear();
        nodes.emplace_back();
        terminal.clear();
        lengths.clear();
        bfs_order.clear();
        built = false;
    }

private:
    std::vector<int> terminal;
    std::vector<std::size_t> lengths;
    std::vector<int> bfs_order;
    bool built = false;

    // 文字の添字を返す。対象範囲外なら -1。O(1)
    static int code(unsigned char ch) {
        const int c = static_cast<int>(ch) - base;
        return 0 <= c && c < char_size ? c : -1;
    }

    // 構築状態と状態番号を検査する。O(1)
    void check_state(int state) const {
        assert(built && "AhoCorasick: call build() after insert()");
        assert(0 <= state && state < size());
        (void)state;
    }
};
