#pragma once

#include <algorithm>
#include <vector>

#include <atcoder/lazysegtree>

using namespace std;

struct RangeSumNode {
    long long sum;
    int len;
};

RangeSumNode range_add_range_sum_op(RangeSumNode a, RangeSumNode b) {
    return {a.sum + b.sum, a.len + b.len};
}

RangeSumNode range_add_range_sum_e() {
    return {0, 0};
}

RangeSumNode range_add_range_sum_mapping(long long f, RangeSumNode x) {
    return {x.sum + f * x.len, x.len};
}

long long range_add_range_sum_composition(long long f, long long g) {
    return f + g;
}

long long range_add_range_sum_id() {
    return 0;
}

using RangeAddRangeSumSegtree = atcoder::lazy_segtree<
    RangeSumNode,
    range_add_range_sum_op,
    range_add_range_sum_e,
    long long,
    range_add_range_sum_mapping,
    range_add_range_sum_composition,
    range_add_range_sum_id>;

struct RangeAddRangeSum {
    RangeAddRangeSumSegtree seg;

    RangeAddRangeSum() = default;

    explicit RangeAddRangeSum(int n) : seg(vector<RangeSumNode>(n, {0, 1})) {}

    explicit RangeAddRangeSum(const vector<long long>& values) : seg(build(values)) {}

    static vector<RangeSumNode> build(const vector<long long>& values) {
        vector<RangeSumNode> init;
        init.reserve(values.size());
        for (long long x : values) init.push_back({x, 1});
        return init;
    }

    void set(int p, long long x) {
        seg.set(p, {x, 1});
    }

    RangeSumNode get_node(int p) {
        return seg.get(p);
    }

    long long get(int p) {
        return seg.get(p).sum;
    }

    void apply(int l, int r, long long x) {
        seg.apply(l, r, x);
    }

    long long prod(int l, int r) {
        return seg.prod(l, r).sum;
    }

    long long all_prod() {
        return seg.all_prod().sum;
    }
};

long long range_add_range_max_op(long long a, long long b) {
    return max(a, b);
}

long long range_add_range_max_e() {
    return -(1LL << 60);
}

long long range_add_range_max_mapping(long long f, long long x) {
    return x + f;
}

long long range_add_range_max_composition(long long f, long long g) {
    return f + g;
}

long long range_add_range_max_id() {
    return 0;
}

using RangeAddRangeMaxSegtree = atcoder::lazy_segtree<
    long long,
    range_add_range_max_op,
    range_add_range_max_e,
    long long,
    range_add_range_max_mapping,
    range_add_range_max_composition,
    range_add_range_max_id>;

struct RangeAddRangeMax {
    RangeAddRangeMaxSegtree seg;

    RangeAddRangeMax() = default;

    explicit RangeAddRangeMax(int n) : seg(vector<long long>(n, 0)) {}

    explicit RangeAddRangeMax(const vector<long long>& values) : seg(values) {}

    void set(int p, long long x) {
        seg.set(p, x);
    }

    long long get(int p) {
        return seg.get(p);
    }

    void apply(int l, int r, long long x) {
        seg.apply(l, r, x);
    }

    long long prod(int l, int r) {
        return seg.prod(l, r);
    }

    long long all_prod() {
        return seg.all_prod();
    }
};

long long range_add_range_min_op(long long a, long long b) {
    return min(a, b);
}

long long range_add_range_min_e() {
    return (1LL << 60);
}

long long range_add_range_min_mapping(long long f, long long x) {
    return x + f;
}

long long range_add_range_min_composition(long long f, long long g) {
    return f + g;
}

long long range_add_range_min_id() {
    return 0;
}

using RangeAddRangeMinSegtree = atcoder::lazy_segtree<
    long long,
    range_add_range_min_op,
    range_add_range_min_e,
    long long,
    range_add_range_min_mapping,
    range_add_range_min_composition,
    range_add_range_min_id>;

struct RangeAddRangeMin {
    RangeAddRangeMinSegtree seg;

    RangeAddRangeMin() = default;

    explicit RangeAddRangeMin(int n) : seg(vector<long long>(n, 0)) {}

    explicit RangeAddRangeMin(const vector<long long>& values) : seg(values) {}

    void set(int p, long long x) {
        seg.set(p, x);
    }

    long long get(int p) {
        return seg.get(p);
    }

    void apply(int l, int r, long long x) {
        seg.apply(l, r, x);
    }

    long long prod(int l, int r) {
        return seg.prod(l, r);
    }

    long long all_prod() {
        return seg.all_prod();
    }
};

struct RangeAssignValue {
    long long value;
    bool has_value;
};

RangeSumNode range_assign_range_sum_op(RangeSumNode a, RangeSumNode b) {
    return {a.sum + b.sum, a.len + b.len};
}

RangeSumNode range_assign_range_sum_e() {
    return {0, 0};
}

RangeSumNode range_assign_range_sum_mapping(RangeAssignValue f, RangeSumNode x) {
    if (!f.has_value) return x;
    return {f.value * x.len, x.len};
}

RangeAssignValue range_assign_range_sum_composition(RangeAssignValue f, RangeAssignValue g) {
    if (f.has_value) return f;
    return g;
}

RangeAssignValue range_assign_range_sum_id() {
    return {0, false};
}

using RangeAssignRangeSumSegtree = atcoder::lazy_segtree<
    RangeSumNode,
    range_assign_range_sum_op,
    range_assign_range_sum_e,
    RangeAssignValue,
    range_assign_range_sum_mapping,
    range_assign_range_sum_composition,
    range_assign_range_sum_id>;

struct RangeAssignRangeSum {
    RangeAssignRangeSumSegtree seg;

    RangeAssignRangeSum() = default;

    explicit RangeAssignRangeSum(int n) : seg(vector<RangeSumNode>(n, {0, 1})) {}

    explicit RangeAssignRangeSum(const vector<long long>& values) : seg(build(values)) {}

    static vector<RangeSumNode> build(const vector<long long>& values) {
        vector<RangeSumNode> init;
        init.reserve(values.size());
        for (long long x : values) init.push_back({x, 1});
        return init;
    }

    void set(int p, long long x) {
        seg.set(p, {x, 1});
    }

    long long get(int p) {
        return seg.get(p).sum;
    }

    void apply(int l, int r, long long x) {
        seg.apply(l, r, {x, true});
    }

    long long prod(int l, int r) {
        return seg.prod(l, r).sum;
    }

    long long all_prod() {
        return seg.all_prod().sum;
    }
};
