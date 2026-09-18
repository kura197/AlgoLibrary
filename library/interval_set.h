#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/interval_set.h

#include <algorithm>
#include <iterator>
#include <set>
#include <utility>

// 半開区間 [left, right) の和集合を管理する
// 区間は互いに交わらず、隣接もしない形に正規化して保持する
template<class T = long long>
struct IntervalSet {
    using Interval = std::pair<T, T>;

    // [left, right) を追加し、新しく覆われた長さを返す: O(log N + K)
    // K は結合によって削除される区間数
    T insert(T left, T right) {
        if (!(left < right)) return T{};

        T before = total_length_;
        typename std::set<Interval>::iterator it = intervals_.lower_bound({left, left});
        if (it != intervals_.begin()) {
            typename std::set<Interval>::iterator previous = std::prev(it);
            if (!(previous->second < left)) it = previous;
        }

        T merged_left = left;
        T merged_right = right;
        while (it != intervals_.end() && !(merged_right < it->first)) {
            merged_left = std::min(merged_left, it->first);
            merged_right = std::max(merged_right, it->second);
            total_length_ -= it->second - it->first;
            it = intervals_.erase(it);
        }

        intervals_.insert({merged_left, merged_right});
        total_length_ += merged_right - merged_left;
        return total_length_ - before;
    }

    // [left, right) を削除し、覆われなくなった長さを返す: O(log N + K)
    // K は削除範囲と交わる区間数
    T erase(T left, T right) {
        if (!(left < right)) return T{};

        T before = total_length_;
        typename std::set<Interval>::iterator it = intervals_.lower_bound({left, left});
        if (it != intervals_.begin()) {
            typename std::set<Interval>::iterator previous = std::prev(it);
            if (left < previous->second) it = previous;
        }

        while (it != intervals_.end() && it->first < right) {
            T interval_left = it->first;
            T interval_right = it->second;
            it = intervals_.erase(it);
            total_length_ -= interval_right - interval_left;

            if (interval_left < left) {
                intervals_.insert({interval_left, left});
                total_length_ += left - interval_left;
            }
            if (right < interval_right) {
                intervals_.insert({right, interval_right});
                total_length_ += interval_right - right;
                break;
            }
        }
        return before - total_length_;
    }

    // point がいずれかの区間に含まれるか: O(log N)
    bool contains(T point) const {
        return containing_interval(point) != intervals_.end();
    }

    // [left, right) 全体が覆われているか: O(log N)
    bool contains(T left, T right) const {
        if (!(left < right)) return true;
        typename std::set<Interval>::const_iterator it = containing_interval(left);
        return it != intervals_.end() && !(it->second < right);
    }

    // point 以上で覆われていない最小値を返す: O(log N)
    T mex(T point) const {
        typename std::set<Interval>::const_iterator it = containing_interval(point);
        if (it == intervals_.end()) return point;
        return it->second;
    }

    // point を含む区間を result に代入する。存在しなければ false
    bool get_interval(T point, Interval& result) const {
        typename std::set<Interval>::const_iterator it = containing_interval(point);
        if (it == intervals_.end()) return false;
        result = *it;
        return true;
    }

    // [left, right) のうち覆われている長さを返す: O(log N + K)
    T covered_length(T left, T right) const {
        if (!(left < right)) return T{};

        T result{};
        typename std::set<Interval>::const_iterator it = intervals_.lower_bound({left, left});
        if (it != intervals_.begin()) {
            typename std::set<Interval>::const_iterator previous = std::prev(it);
            if (left < previous->second) it = previous;
        }

        while (it != intervals_.end() && it->first < right) {
            T overlap_left = std::max(left, it->first);
            T overlap_right = std::min(right, it->second);
            if (overlap_left < overlap_right) result += overlap_right - overlap_left;
            ++it;
        }
        return result;
    }

    T total_length() const {
        return total_length_;
    }

    int interval_count() const {
        return (int)intervals_.size();
    }

    bool empty() const {
        return intervals_.empty();
    }

    const std::set<Interval>& intervals() const {
        return intervals_;
    }

    void clear() {
        intervals_.clear();
        total_length_ = T{};
    }

private:
    std::set<Interval> intervals_;
    T total_length_{};

    typename std::set<Interval>::const_iterator containing_interval(T point) const {
        typename std::set<Interval>::const_iterator it = intervals_.lower_bound({point, point});
        if (it != intervals_.end() && it->first == point) return it;
        if (it == intervals_.begin()) return intervals_.end();
        --it;
        if (point < it->second) return it;
        return intervals_.end();
    }
};

template<class T = long long>
using RangeSet = IntervalSet<T>;
