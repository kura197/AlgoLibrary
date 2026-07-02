
#pragma once

#include <cmath>
#include <complex>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;


// 3点を通る円の中心座標(外心)と半径を返す
tuple<double, double, double> circumcircle(double x1, double y1, double x2, double y2, double x3, double y3){
    double xp, yp, rp;

    xp = ((y1-y2)*(x3*x3-x1*x1+y3*y3-y1*y1) - (y1-y3)*(x2*x2-x1*x1+y2*y2-y1*y1)) / (2*(x1-x2)*(y1-y3) - 2*(x1-x3)*(y1-y2));
    yp = ((x1-x3)*(x2*x2-x1*x1+y2*y2-y1*y1) - (x1-x2)*(x3*x3-x1*x1+y3*y3-y1*y1)) / (2*(x1-x2)*(y1-y3) - 2*(x1-x3)*(y1-y2));
    rp = sqrt((xp-x1)*(xp-x1) + (yp-y1)*(yp-y1));

    return make_tuple(xp, yp, rp);
}

// 2点間距離を返す
double distance(double x1, double y1, double x2, double y2){
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}


// 原点まわりに rot [rad] 回転する
complex<double> rotate(complex<double> pos, double rot){
    const double EPS = 1e-12;
    auto r = complex<double>(cos(rot), sin(rot));
    pos *= r;
    double y = pos.imag();
    double x = pos.real();
    if(abs(y) < EPS) y = 0;
    if(abs(x) < EPS) x = 0;
    return complex<double>(x, y);
}

// 原点まわりに rot [rad] 回転する
pair<double, double> rotate(pair<double, double> pos, double rot){
    const double EPS = 1e-12;
    double x = pos.first;
    double y = pos.second;
 
    double nx = x * cos(rot) - y * sin(rot);
    double ny = y * cos(rot) + x * sin(rot);
 
    if(abs(ny) < EPS) ny = 0;
    if(abs(nx) < EPS) nx = 0;
 
    return make_pair(nx, ny);
}


// 点集合の重心を返す
pair<double, double> centroid(const vector<pair<double, double>>& points){
    long double gx = 0, gy = 0;
    for(auto& [x, y] : points){
        gx += x;
        gy += y;
    }
    gx /= points.size();
    gy /= points.size();
    return make_pair((double)gx, (double)gy);
}

// 点 center から見たベクトル center->a と center->b のなす角を返す
long double angle(pair<double, double> a, pair<double, double> b, pair<double, double> center){
    const long double EPS = 1e-12;
    a.first -= center.first, a.second -= center.second;
    b.first -= center.first, b.second -= center.second;
    long double cos_theta = a.first * b.first + a.second * b.second;
    cos_theta /= (EPS + sqrt(a.first * a.first + a.second * a.second));
    cos_theta /= (EPS + sqrt(b.first * b.first + b.second * b.second));
    cos_theta = max((long double)-1.0, min((long double)1.0, cos_theta));
    return acos(cos_theta);
}

// 点 center を中心に point を theta [rad] 回転した点を返す
pair<double, double> rotate(pair<double, double> point, pair<double, double> center, double theta){
    const double EPS = 1e-12;
    point.first -= center.first, point.second -= center.second;
    point = pair<double, double>{point.first*cos(theta) - point.second*sin(theta), point.first*sin(theta) + point.second*cos(theta)};
    point.first += center.first, point.second += center.second;
    if(abs(point.first) < EPS) point.first = 0;
    if(abs(point.second) < EPS) point.second = 0;
    return point;
}
