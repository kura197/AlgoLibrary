#pragma once

struct point3 {
    long long x, y, z;
};

point3 operator+(const point3& a, const point3& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

point3 operator-(const point3& a, const point3& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

// 3次元ベクトルa, bの内積を計算する
long long dot(const point3& a, const point3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// 3次元ベクトルa, bの外積を計算する
point3 cross(const point3& a, const point3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

// 3次元ベクトルの長さの2乗を計算する
long long norm2(const point3& a) {
    return dot(a, a);
}

// 3次元ベクトルの長さを計算する
double norm(const point3& a) {
    return sqrt((double)norm2(a));
}

// 2つの3次元ベクトルのなす角をラジアンで計算する
double get_radian(const point3& a, const point3& b) {
    return atan2(norm(cross(a, b)), (double)dot(a, b));
}

// 2つの3次元ベクトルのなす角を度数法で計算する
double get_degree(const point3& a, const point3& b) {
    return get_radian(a, b) * 180.0 / acos(-1);
}

// 3点A, B, Cが作る角ABCをラジアンで計算する
double angle_radian(const point3& A, const point3& B, const point3& C) {
    return get_radian(A - B, C - B);
}

// 3点A, B, Cが作る角ABCを度数法で計算する
double angle_degree(const point3& A, const point3& B, const point3& C) {
    return angle_radian(A, B, C) * 180.0 / acos(-1);
}

// 法線normalから見た、ベクトルaからbへの符号付き角をラジアンで計算する
double get_signed_radian(const point3& a, const point3& b, const point3& normal) {
    return atan2((double)dot(normal, cross(a, b)), (double)dot(a, b));
}

// 法線normalから見た、ベクトルaからbへの符号付き角を度数法で計算する
double get_signed_degree(const point3& a, const point3& b, const point3& normal) {
    return get_signed_radian(a, b, normal) * 180.0 / acos(-1);
}
