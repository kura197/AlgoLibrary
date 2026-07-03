#pragma once

#include <algorithm>

#include <atcoder/segtree>

using namespace std;

long long rmq_op(long long a, long long b) {
    return min(a, b);
}

long long rmq_e() {
    return (1LL << 60);
}

using RMQ = atcoder::segtree<long long, rmq_op, rmq_e>;

long long rsq_op(long long a, long long b) {
    return a + b;
}

long long rsq_e() {
    return 0;
}

using RSQ = atcoder::segtree<long long, rsq_op, rsq_e>;

long long rmaxq_op(long long a, long long b) {
    return max(a, b);
}

long long rmaxq_e() {
    return -(1LL << 60);
}

using RMaxQ = atcoder::segtree<long long, rmaxq_op, rmaxq_e>;
