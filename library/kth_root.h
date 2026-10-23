#pragma once

#include <cassert>
#include <climits>

namespace kth_root_detail {

using ull = unsigned long long;

// base^exponent と target を比較する。積が ull に収まらなくてもよい。
inline int compare_power(ull base, long long exponent, ull target) {
    if (base == 0) return target == 0 ? 0 : -1;
    if (base == 1) return target == 1 ? 0 : (target < 1 ? 1 : -1);

    ull result = 1;
    for (long long i = 0; i < exponent; ++i) {
        if (result > target / base) return 1;
        result *= base;
    }
    if (result < target) return -1;
    if (result > target) return 1;
    return 0;
}

inline ull floor_nonnegative(ull value, long long degree) {
    if (value <= 1 || degree == 1) return value;

    ull ok = 1;
    ull ng = value;
    while (ok < ng) {
        const ull mid = ok + (ng - ok + 1) / 2;
        if (compare_power(mid, degree, value) <= 0) {
            ok = mid;
        } else {
            ng = mid - 1;
        }
    }
    return ok;
}

inline ull ceil_nonnegative(ull value, long long degree) {
    const ull root = floor_nonnegative(value, degree);
    return root + (compare_power(root, degree, value) != 0);
}

inline ull magnitude(long long value) {
    if (value >= 0) return static_cast<ull>(value);
    return 0ULL - static_cast<ull>(value);
}

inline long long negative_from_magnitude(ull value) {
    const ull min_magnitude = static_cast<ull>(LLONG_MAX) + 1;
    if (value == min_magnitude) return LLONG_MIN;
    return -static_cast<long long>(value);
}

}  // namespace kth_root_detail

// value の実数 k 乗根を切り捨てた整数を返す。
// degree > 0 が必要。value < 0 のときは degree が奇数である必要がある。
inline long long floor_kth_root(long long value, long long degree) {
    assert(degree > 0);
    if (value >= 0) {
        return static_cast<long long>(
            kth_root_detail::floor_nonnegative(value, degree));
    }

    assert(degree % 2 == 1);
    return kth_root_detail::negative_from_magnitude(
        kth_root_detail::ceil_nonnegative(
            kth_root_detail::magnitude(value), degree));
}

// value の実数 k 乗根を切り上げた整数を返す。
// degree > 0 が必要。value < 0 のときは degree が奇数である必要がある。
inline long long ceil_kth_root(long long value, long long degree) {
    assert(degree > 0);
    if (value >= 0) {
        return static_cast<long long>(
            kth_root_detail::ceil_nonnegative(value, degree));
    }

    assert(degree % 2 == 1);
    return kth_root_detail::negative_from_magnitude(
        kth_root_detail::floor_nonnegative(
            kth_root_detail::magnitude(value), degree));
}

// value が整数の完全 k 乗なら true を返す。
// 負数は degree が奇数のときだけ完全 k 乗になり得る。
inline bool is_perfect_kth_power(long long value, long long degree) {
    assert(degree > 0);
    if (value < 0 && degree % 2 == 0) return false;

    const auto magnitude = kth_root_detail::magnitude(value);
    const auto root = kth_root_detail::floor_nonnegative(magnitude, degree);
    return kth_root_detail::compare_power(root, degree, magnitude) == 0;
}

// floor_kth_root の短い別名。
inline long long kth_root(long long value, long long degree) {
    return floor_kth_root(value, degree);
}
