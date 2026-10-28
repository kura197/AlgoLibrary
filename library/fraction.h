#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/fraction.h

#include <cassert>
#include <climits>
#include <istream>
#include <ostream>

// long long で表現する有理数。
// 常に q > 0、gcd(|p|, q) = 1 を満たし、0 は 0/1 として保持する。
struct fraction {
    long long p;  // 分子
    long long q;  // 分母

    fraction(long long numerator = 0, long long denominator = 1) {
        i128 numerator128 = numerator;
        i128 denominator128 = denominator;
        normalize(numerator128, denominator128);
        assert(fits_long_long(numerator128));
        assert(fits_long_long(denominator128));
        p = static_cast<long long>(numerator128);
        q = static_cast<long long>(denominator128);
    }

    fraction operator+() const {
        return *this;
    }

    fraction operator-() const {
        return from_int128(-static_cast<i128>(p), q);
    }

    fraction& operator+=(const fraction& other) {
        return *this = *this + other;
    }

    fraction& operator-=(const fraction& other) {
        return *this = *this - other;
    }

    fraction& operator*=(const fraction& other) {
        return *this = *this * other;
    }

    fraction& operator/=(const fraction& other) {
        return *this = *this / other;
    }

    friend fraction operator+(const fraction& lhs, const fraction& rhs) {
        return from_int128(
            static_cast<i128>(lhs.p) * rhs.q
                + static_cast<i128>(rhs.p) * lhs.q,
            static_cast<i128>(lhs.q) * rhs.q);
    }

    friend fraction operator-(const fraction& lhs, const fraction& rhs) {
        return from_int128(
            static_cast<i128>(lhs.p) * rhs.q
                - static_cast<i128>(rhs.p) * lhs.q,
            static_cast<i128>(lhs.q) * rhs.q);
    }

    friend fraction operator*(const fraction& lhs, const fraction& rhs) {
        return from_int128(
            static_cast<i128>(lhs.p) * rhs.p,
            static_cast<i128>(lhs.q) * rhs.q);
    }

    friend fraction operator/(const fraction& lhs, const fraction& rhs) {
        assert(rhs.p != 0);
        return from_int128(
            static_cast<i128>(lhs.p) * rhs.q,
            static_cast<i128>(lhs.q) * rhs.p);
    }

    friend bool operator==(const fraction& lhs, const fraction& rhs) {
        return lhs.p == rhs.p && lhs.q == rhs.q;
    }

    friend bool operator!=(const fraction& lhs, const fraction& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const fraction& lhs, const fraction& rhs) {
        return static_cast<i128>(lhs.p) * rhs.q
             < static_cast<i128>(rhs.p) * lhs.q;
    }

    friend bool operator<=(const fraction& lhs, const fraction& rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>(const fraction& lhs, const fraction& rhs) {
        return rhs < lhs;
    }

    friend bool operator>=(const fraction& lhs, const fraction& rhs) {
        return !(lhs < rhs);
    }

    // 逆数を返す。0 には使用できない。
    fraction inverse() const {
        assert(p != 0);
        return from_int128(q, p);
    }

    // 自身以下の最大の整数を返す。
    long long floor() const {
        const long long quotient = p / q;
        const long long remainder = p % q;
        return quotient - (remainder < 0);
    }

    // 自身以上の最小の整数を返す。
    long long ceil() const {
        const long long quotient = p / q;
        const long long remainder = p % q;
        return quotient + (remainder > 0);
    }

    bool is_integer() const {
        return q == 1;
    }

    // 負、0、正に応じて -1、0、1 を返す。
    int sign() const {
        return (p > 0) - (p < 0);
    }

    fraction abs() const {
        return p < 0 ? from_int128(-static_cast<i128>(p), q) : *this;
    }

    long double value() const {
        return static_cast<long double>(p) / q;
    }

    friend fraction abs(const fraction& value) {
        return value.abs();
    }

    // "p/q" 形式で出力し、"p/q" または整数 "p" 形式で入力する。
    friend std::ostream& operator<<(std::ostream& os, const fraction& value) {
        return os << value.p << '/' << value.q;
    }

    friend std::istream& operator>>(std::istream& is, fraction& value) {
        long long numerator;
        if (!(is >> numerator)) return is;

        long long denominator = 1;
        if (is.peek() == '/') {
            is.get();
            if (!(is >> denominator)) return is;
        }
        value = fraction(numerator, denominator);
        return is;
    }

private:
    using i128 = __int128_t;

    struct normalized_tag {};

    fraction(long long numerator, long long denominator, normalized_tag)
        : p(numerator), q(denominator) {}

    static i128 abs128(i128 value) {
        return value < 0 ? -value : value;
    }

    static i128 gcd128(i128 lhs, i128 rhs) {
        lhs = abs128(lhs);
        rhs = abs128(rhs);
        while (rhs != 0) {
            const i128 remainder = lhs % rhs;
            lhs = rhs;
            rhs = remainder;
        }
        return lhs;
    }

    static void normalize(i128& numerator, i128& denominator) {
        assert(denominator != 0);
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
        if (numerator == 0) {
            denominator = 1;
            return;
        }

        const i128 divisor = gcd128(numerator, denominator);
        numerator /= divisor;
        denominator /= divisor;
    }

    static bool fits_long_long(i128 value) {
        return static_cast<i128>(LLONG_MIN) <= value
            && value <= static_cast<i128>(LLONG_MAX);
    }

    static fraction from_int128(i128 numerator, i128 denominator) {
        normalize(numerator, denominator);
        assert(fits_long_long(numerator));
        assert(fits_long_long(denominator));
        return fraction(
            static_cast<long long>(numerator),
            static_cast<long long>(denominator),
            normalized_tag{});
    }
};

using Fraction = fraction;
