#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>
#include <type_traits>

using namespace std;

namespace internal_mat {

template<class T, class = void>
struct has_val_method : false_type {};

template<class T>
struct has_val_method<T, void_t<decltype(declval<const T&>().val())>> : true_type {};

}  // namespace internal_mat

template<class T>
struct Vector {
    vector<T> data;

    Vector() = default;
    explicit Vector(int n) : data(n) {}
    Vector(int n, const T& value) : data(n, value) {}
    explicit Vector(vector<T> values) : data(std::move(values)) {}

    int size() const {
        return (int)data.size();
    }

    T& operator[](int i) {
        return data[i];
    }

    const T& operator[](int i) const {
        return data[i];
    }

    void print(ostream& os = cout) const {
        for (int i = 0; i < size(); i++) {
            if (i) os << ' ';
            if constexpr (internal_mat::has_val_method<T>::value) {
                os << data[i].val();
            } else {
                os << data[i];
            }
        }
        os << '\n';
    }

    template<class Printer>
    void print_with(Printer printer, ostream& os = cout) const {
        for (int i = 0; i < size(); i++) {
            if (i) os << ' ';
            printer(os, data[i]);
        }
        os << '\n';
    }
};

template<class T>
struct Matrix {
    vector<Vector<T>> data;

    Matrix() = default;
    Matrix(int height, int width) : data(height, Vector<T>(width)) {}
    Matrix(int height, int width, const T& value) : data(height, Vector<T>(width, value)) {}
    explicit Matrix(vector<Vector<T>> values) : data(std::move(values)) {}

    int height() const {
        return (int)data.size();
    }

    int width() const {
        return data.empty() ? 0 : data[0].size();
    }

    Vector<T>& operator[](int i) {
        return data[i];
    }

    const Vector<T>& operator[](int i) const {
        return data[i];
    }

    static Matrix identity(int n) {
        Matrix ret(n, n, T(0));
        for (int i = 0; i < n; i++) ret[i][i] = T(1);
        return ret;
    }

    void print(ostream& os = cout) const {
        for (const auto& row : data) row.print(os);
    }

    template<class Printer>
    void print_with(Printer printer, ostream& os = cout) const {
        for (const auto& row : data) row.print_with(printer, os);
    }
};

template<class T>
Matrix<T> mat_mul(const Matrix<T>& a, const Matrix<T>& b) {
    assert(a.width() == b.height());
    Matrix<T> c(a.height(), b.width(), T(0));
    for (int i = 0; i < a.height(); i++) {
        for (int k = 0; k < a.width(); k++) {
            for (int j = 0; j < b.width(); j++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

template<class T>
Vector<T> mat_mul(const Matrix<T>& a, const Vector<T>& b) {
    assert(a.width() == b.size());
    Vector<T> c(a.height(), T(0));
    for (int i = 0; i < a.height(); i++) {
        for (int j = 0; j < b.size(); j++) {
            c[i] += a[i][j] * b[j];
        }
    }
    return c;
}

template<class T>
Matrix<T> mat_pow(Matrix<T> a, long long n) {
    assert(a.height() == a.width());
    Matrix<T> ret = Matrix<T>::identity(a.height());
    while (n > 0) {
        if (n & 1) ret = mat_mul(ret, a);
        a = mat_mul(a, a);
        n >>= 1;
    }
    return ret;
}
