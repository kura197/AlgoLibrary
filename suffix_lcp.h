#include <algorithm>
#include <string>
#include <vector>
#include <atcoder/all>

// ACLのsuffix_array/lcp_arrayを使った接尾辞配列ラッパー
// 構築 O(N log N), contain O(|T| log N), get_lcp O(1)
struct SuffixLCP {
    std::string s;
    std::vector<int> sa;
    std::vector<int> lcp;
    std::vector<int> rank;

    SuffixLCP(const std::string& s) : s(s) {
        sa = atcoder::suffix_array(s);
        lcp = atcoder::lcp_array(s, sa);
        rank.assign(s.size(), 0);
        for (int i = 0; i < (int)sa.size(); i++) {
            rank[sa[i]] = i;
        }
        build_sparse_table();
    }

    // 文字列tがsに含まれるかを判定する O(|T| log N)
    bool contain(const std::string& t) const {
        int low = 0;
        int high = (int)sa.size();
        while (low < high) {
            int mid = (low + high) / 2;
            if (s.compare(sa[mid], t.size(), t) < 0) low = mid + 1;
            else high = mid;
        }
        return low < (int)sa.size() && s.compare(sa[low], t.size(), t) == 0;
    }

    // sa上のi番目とj番目のsuffixのLCPを返す O(1)
    int get_lcp_by_sa_index(int i, int j) const {
        if (i == j) return (int)s.size() - sa[i];
        if (i > j) std::swap(i, j);
        return range_min(i, j);
    }

    // s[i..]とs[j..]のLCPを返す O(1)
    int get_lcp_by_string_index(int i, int j) const {
        return get_lcp_by_sa_index(rank[i], rank[j]);
    }

    // sa上のi番目とj番目のsuffixについて、元文字列上の開始位置の差を返す O(1)
    int get_dist_by_sa_index(int i, int j) const {
        int x = sa[i];
        int y = sa[j];
        if (x > y) std::swap(x, y);
        return y - x;
    }

private:
    std::vector<int> log_table;
    std::vector<std::vector<int>> sparse_table;

    void build_sparse_table() {
        int n = (int)lcp.size();
        log_table.assign(n + 1, 0);
        for (int i = 2; i <= n; i++) {
            log_table[i] = log_table[i / 2] + 1;
        }

        int height = n == 0 ? 0 : log_table[n] + 1;
        sparse_table.assign(height, std::vector<int>(n));
        if (n == 0) return;

        sparse_table[0] = lcp;
        for (int k = 1; k < height; k++) {
            int len = 1 << k;
            for (int i = 0; i + len <= n; i++) {
                sparse_table[k][i] = std::min(sparse_table[k - 1][i], sparse_table[k - 1][i + (len >> 1)]);
            }
        }
    }

    int range_min(int left, int right) const {
        int len = right - left;
        int k = log_table[len];
        return std::min(sparse_table[k][left], sparse_table[k][right - (1 << k)]);
    }
};
