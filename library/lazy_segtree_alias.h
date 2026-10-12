#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/lazy_segtree_alias.h

#include <algorithm>
#include <limits>
#include <vector>

#include <atcoder/lazysegtree>

using namespace std;

template<class T>
struct RangeSumNode {
    T sum;
    int len;
};

template<class T>
RangeSumNode<T> range_add_range_sum_op(RangeSumNode<T> a, RangeSumNode<T> b) {
    return {a.sum + b.sum, a.len + b.len};
}

template<class T>
RangeSumNode<T> range_add_range_sum_e() {
    return {T{}, 0};
}

template<class T>
RangeSumNode<T> range_add_range_sum_mapping(T f, RangeSumNode<T> x) {
    return {x.sum + f * x.len, x.len};
}

template<class T>
T range_add_range_sum_composition(T f, T g) {
    return f + g;
}

template<class T>
T range_add_range_sum_id() {
    return T{};
}

template<class T>
using RangeAddRangeSumSegtree = atcoder::lazy_segtree<
    RangeSumNode<T>,
    range_add_range_sum_op<T>,
    range_add_range_sum_e<T>,
    T,
    range_add_range_sum_mapping<T>,
    range_add_range_sum_composition<T>,
    range_add_range_sum_id<T>>;

template<class T>
struct RangeAddRangeSum {
    RangeAddRangeSumSegtree<T> seg;

    RangeAddRangeSum() = default;

    explicit RangeAddRangeSum(int n) : seg(n) {}

    explicit RangeAddRangeSum(const vector<T>& values) : seg(build(values)) {}

    static vector<RangeSumNode<T>> build(const vector<T>& values) {
        vector<RangeSumNode<T>> init;
        init.reserve(values.size());
        for (T x : values) init.push_back({x, 1});
        return init;
    }

    void set(int p, T x) {
        seg.set(p, {x, 1});
    }

    RangeSumNode<T> get_node(int p) {
        return seg.get(p);
    }

    T get(int p) {
        return seg.get(p).sum;
    }

    void apply(int l, int r, T x) {
        seg.apply(l, r, x);
    }

    T prod(int l, int r) {
        return seg.prod(l, r).sum;
    }

    T all_prod() {
        return seg.all_prod().sum;
    }
};

template<class T>
T range_add_range_max_op(T a, T b) {
    return max(a, b);
}

template<class T>
T range_add_range_max_e() {
    return numeric_limits<T>::lowest();
}

template<class T>
T range_add_range_max_mapping(T f, T x) {
    if (x == range_add_range_max_e<T>()) return x;
    return x + f;
}

template<class T>
T range_add_range_max_composition(T f, T g) {
    return f + g;
}

template<class T>
T range_add_range_max_id() {
    return T{};
}

template<class T>
using RangeAddRangeMaxSegtree = atcoder::lazy_segtree<
    T,
    range_add_range_max_op<T>,
    range_add_range_max_e<T>,
    T,
    range_add_range_max_mapping<T>,
    range_add_range_max_composition<T>,
    range_add_range_max_id<T>>;

template<class T>
struct RangeAddRangeMax {
    RangeAddRangeMaxSegtree<T> seg;

    RangeAddRangeMax() = default;

    explicit RangeAddRangeMax(int n) : seg(n) {}

    explicit RangeAddRangeMax(const vector<T>& values) : seg(values) {}

    void set(int p, T x) {
        seg.set(p, x);
    }

    T get(int p) {
        return seg.get(p);
    }

    void apply(int l, int r, T x) {
        seg.apply(l, r, x);
    }

    T prod(int l, int r) {
        return seg.prod(l, r);
    }

    T all_prod() {
        return seg.all_prod();
    }
};

template<class T>
T range_add_range_min_op(T a, T b) {
    return min(a, b);
}

template<class T>
T range_add_range_min_e() {
    return numeric_limits<T>::max();
}

template<class T>
T range_add_range_min_mapping(T f, T x) {
    if (x == range_add_range_min_e<T>()) return x;
    return x + f;
}

template<class T>
T range_add_range_min_composition(T f, T g) {
    return f + g;
}

template<class T>
T range_add_range_min_id() {
    return T{};
}

template<class T>
using RangeAddRangeMinSegtree = atcoder::lazy_segtree<
    T,
    range_add_range_min_op<T>,
    range_add_range_min_e<T>,
    T,
    range_add_range_min_mapping<T>,
    range_add_range_min_composition<T>,
    range_add_range_min_id<T>>;

template<class T>
struct RangeAddRangeMin {
    RangeAddRangeMinSegtree<T> seg;

    RangeAddRangeMin() = default;

    explicit RangeAddRangeMin(int n) : seg(n) {}

    explicit RangeAddRangeMin(const vector<T>& values) : seg(values) {}

    void set(int p, T x) {
        seg.set(p, x);
    }

    T get(int p) {
        return seg.get(p);
    }

    void apply(int l, int r, T x) {
        seg.apply(l, r, x);
    }

    T prod(int l, int r) {
        return seg.prod(l, r);
    }

    T all_prod() {
        return seg.all_prod();
    }
};

template<class T>
struct RangeAssignValue {
    T value;
    bool has_value;
};

template<class T>
RangeSumNode<T> range_assign_range_sum_op(RangeSumNode<T> a, RangeSumNode<T> b) {
    return {a.sum + b.sum, a.len + b.len};
}

template<class T>
RangeSumNode<T> range_assign_range_sum_e() {
    return {T{}, 0};
}

template<class T>
RangeSumNode<T> range_assign_range_sum_mapping(
    RangeAssignValue<T> f,
    RangeSumNode<T> x
) {
    if (!f.has_value) return x;
    return {f.value * x.len, x.len};
}

template<class T>
RangeAssignValue<T> range_assign_range_sum_composition(
    RangeAssignValue<T> f,
    RangeAssignValue<T> g
) {
    if (f.has_value) return f;
    return g;
}

template<class T>
RangeAssignValue<T> range_assign_range_sum_id() {
    return {T{}, false};
}

template<class T>
using RangeAssignRangeSumSegtree = atcoder::lazy_segtree<
    RangeSumNode<T>,
    range_assign_range_sum_op<T>,
    range_assign_range_sum_e<T>,
    RangeAssignValue<T>,
    range_assign_range_sum_mapping<T>,
    range_assign_range_sum_composition<T>,
    range_assign_range_sum_id<T>>;

template<class T>
struct RangeAssignRangeSum {
    RangeAssignRangeSumSegtree<T> seg;

    RangeAssignRangeSum() = default;

    explicit RangeAssignRangeSum(int n) : seg(n) {}

    explicit RangeAssignRangeSum(const vector<T>& values) : seg(build(values)) {}

    static vector<RangeSumNode<T>> build(const vector<T>& values) {
        vector<RangeSumNode<T>> init;
        init.reserve(values.size());
        for (T x : values) init.push_back({x, 1});
        return init;
    }

    void set(int p, T x) {
        seg.set(p, {x, 1});
    }

    T get(int p) {
        return seg.get(p).sum;
    }

    void apply(int l, int r, T x) {
        seg.apply(l, r, {x, true});
    }

    T prod(int l, int r) {
        return seg.prod(l, r).sum;
    }

    T all_prod() {
        return seg.all_prod().sum;
    }
};

// 一次変換 x -> a * x + b
template<class T>
struct AffineTransformation {
    T a;
    T b;
};

template<class T>
RangeSumNode<T> range_affine_range_sum_mapping(
    AffineTransformation<T> f,
    RangeSumNode<T> x
) {
    return {f.a * x.sum + f.b * x.len, x.len};
}

// f(g(x)) となるように合成する
template<class T>
AffineTransformation<T> range_affine_range_sum_composition(
    AffineTransformation<T> f,
    AffineTransformation<T> g
) {
    return {f.a * g.a, f.a * g.b + f.b};
}

template<class T>
AffineTransformation<T> range_affine_range_sum_id() {
    return {T{1}, T{}};
}

template<class T>
using RangeAffineRangeSumSegtree = atcoder::lazy_segtree<
    RangeSumNode<T>,
    range_add_range_sum_op<T>,
    range_add_range_sum_e<T>,
    AffineTransformation<T>,
    range_affine_range_sum_mapping<T>,
    range_affine_range_sum_composition<T>,
    range_affine_range_sum_id<T>>;

// 使い方:
// RangeAffineRangeSum<long long> seg(vector<long long>{1, 2, 3});
// seg.apply(0, 2, 3, 4); // [0, 2) の各 x を 3 * x + 4 に更新
// long long sum = seg.prod(0, 3);
template<class T>
struct RangeAffineRangeSum {
    RangeAffineRangeSumSegtree<T> seg;

    RangeAffineRangeSum() = default;

    explicit RangeAffineRangeSum(int n) : seg(n) {}

    explicit RangeAffineRangeSum(const vector<T>& values) : seg(build(values)) {}

    static vector<RangeSumNode<T>> build(const vector<T>& values) {
        vector<RangeSumNode<T>> init;
        init.reserve(values.size());
        for (T x : values) init.push_back({x, 1});
        return init;
    }

    void set(int p, T x) {
        seg.set(p, {x, 1});
    }

    T get(int p) {
        return seg.get(p).sum;
    }

    void apply(int l, int r, AffineTransformation<T> f) {
        seg.apply(l, r, f);
    }

    void apply(int l, int r, T a, T b) {
        apply(l, r, {a, b});
    }

    T prod(int l, int r) {
        return seg.prod(l, r).sum;
    }

    T all_prod() {
        return seg.all_prod().sum;
    }
};
