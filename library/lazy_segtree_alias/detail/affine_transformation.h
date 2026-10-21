#pragma once

// 一次変換 x -> a * x + b
template <class T>
struct AffineTransformation {
    T a;
    T b;
};

// f(g(x)) となるように合成する。
template <class T>
AffineTransformation<T> affine_composition(
    AffineTransformation<T> f,
    AffineTransformation<T> g
) {
    return {f.a * g.a, f.a * g.b + f.b};
}

template <class T>
AffineTransformation<T> affine_id() {
    return {T{1}, T{}};
}
