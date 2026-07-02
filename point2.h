#pragma once

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

using namespace std;

// check by tenkei-009: https://atcoder.jp/contests/typical90/submissions/76743434

struct point {
    long long x, y;
};

bool operator<(const point &a, const point &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

bool operator==(const point &a, const point &b) {
    return a.x == b.x && a.y == b.y;
}

// 2次元ベクトルa, bの外積を計算する
long long cross(const point &a, const point &b) {
    return (long long) a.x * b.y - (long long) a.y * b.x;
}

// 3点a, b, cについて、ベクトルabとacの外積を計算する
long long cross(const point &a, const point &b, const point &c) {
    return (long long)(b.x - a.x) * (c.y - a.y) - (long long)(b.y - a.y) * (c.x - a.x);
}

// 2次元ベクトルa, bの内積を計算する
long long dot(const point &a, const point &b) {
    return (long long) a.x * b.x + (long long) a.y * b.y;
}

// 偏角順ソート用に、半平面と外積で2点の順序を比較する
bool cmp(const point &a, const point &b) {
    int ah = (a.y < 0 or (a.y == 0 and a.x < 0));
    int bh = (b.y < 0 or (b.y == 0 and b.x < 0));
    if (ah != bh) return ah < bh;
    return cross(a, b) > 0;
}

// 原点まわりの偏角順に点列をソートする
void argument_sort(vector<point> &points) {
    sort(points.begin(), points.end(), cmp);
}

// ベクトルaからbへの有向角をラジアンで計算する
double get_radian(point a, point b) {
    double rad = atan2(cross(a, b), dot(a, b)); // [-pi, pi]
    return rad;
}

// ベクトルaからbへの有向角を度数法で計算する
double get_degree(point a, point b) {
    return get_radian(a, b) * 180.0 / acos(-1); // [-180, 180]
}

// 多角形の符号付き2倍面積を計算する
long long signed_area2(const vector<point> &polygon) {
    int n = polygon.size();
    long long area2 = 0;
    for (int i = 0; i < n; i++) {
        const point &a = polygon[i];
        const point &b = polygon[(i + 1) % n];
        area2 += cross(a, b);
    }
    return area2;
}

// 多角形の2倍面積を計算する
long long polygon_area2(const vector<point> &polygon) {
    long long area2 = signed_area2(polygon);
    if (area2 < 0) area2 *= -1;
    return area2;
}

// 多角形の境界上にある格子点数を計算する
long long lattice_points_on_polygon(const vector<point> &polygon) {
    int n = polygon.size();
    long long boundary = 0;
    for (int i = 0; i < n; i++) {
        long long dx = (long long)polygon[(i + 1) % n].x - polygon[i].x;
        long long dy = (long long)polygon[(i + 1) % n].y - polygon[i].y;
        if (dx < 0) dx *= -1;
        if (dy < 0) dy *= -1;
        boundary += gcd(dx, dy);
    }
    return boundary;
}

// Pickの定理を用いて、多角形の内部にある格子点数を計算する
long long lattice_points_inside_polygon(const vector<point> &polygon) {
    long long area2 = polygon_area2(polygon);
    long long boundary = lattice_points_on_polygon(polygon);
    return (area2 - boundary + 2) / 2;
}

// Pickの定理を用いて、多角形の内部または境界上にある格子点数を計算する
long long lattice_points_on_or_inside_polygon(const vector<point> &polygon) {
    long long area2 = polygon_area2(polygon);
    long long boundary = lattice_points_on_polygon(polygon);
    return (area2 + boundary + 2) / 2;
}

// 点集合の凸包を反時計回り順に計算する
vector<point> convex_hull(vector<point> points, bool include_collinear = false) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    int n = points.size();
    if (n <= 1) return points;

    vector<point> lower, upper;
    for (const point &p : points) {
        while ((int)lower.size() >= 2) {
            long long cr = cross(lower[(int)lower.size() - 2], lower[(int)lower.size() - 1], p);
            if (include_collinear ? cr < 0 : cr <= 0) lower.pop_back();
            else break;
        }
        lower.push_back(p);
    }

    for (int i = n - 1; i >= 0; i--) {
        const point &p = points[i];
        while ((int)upper.size() >= 2) {
            long long cr = cross(upper[(int)upper.size() - 2], upper[(int)upper.size() - 1], p);
            if (include_collinear ? cr < 0 : cr <= 0) upper.pop_back();
            else break;
        }
        upper.push_back(p);
    }

    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}
