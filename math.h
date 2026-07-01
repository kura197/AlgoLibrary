
#pragma once


/// 3点を通る中心座標(外心)、半径を求める
tuple<double, double, double> calc_circle(double x1, double y1, double x2, double y2, double x3, double y3){
    double xp, yp, rp;

    xp = ((y1-y2)*(x3*x3-x1*x1+y3*y3-y1*y1) - (y1-y3)*(x2*x2-x1*x1+y2*y2-y1*y1)) / (2*(x1-x2)*(y1-y3) - 2*(x1-x3)*(y1-y2));
    yp = ((x1-x3)*(x2*x2-x1*x1+y2*y2-y1*y1) - (x1-x2)*(x3*x3-x1*x1+y3*y3-y1*y1)) / (2*(x1-x2)*(y1-y3) - 2*(x1-x3)*(y1-y2));
    rp = sqrt((xp-x1)*(xp-x1) + (yp-y1)*(yp-y1));

    return make_tuple(xp, yp, rp);
}

/// 距離を算出
double calc_dist(double x1, double y1, double x2, double y2){
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}


/// rot [rad] 回転
/// https://atcoder.jp/contests/abc207/tasks/abc207_d
complex<double> rotate(complex<double> pos, double rot){
    auto r = complex<double>(cos(rot), sin(rot));
    pos *= r;
    double y = pos.imag();
    double x = pos.real();
    //if(abs(y) < EPS) y = 0;
    //if(abs(x) < EPS) x = 0;
    return complex<double>(x, y);
}

/// rot [rad] 回転
pair<double, double> rotate(pair<double, double> pos, double rot){
    double x = pos.first;
    double y = pos.second;
 
    double nx = x * cos(rot) - y * sin(rot);
    double ny = y * cos(rot) + x * sin(rot);
 
    if(abs(ny) < EPS) ny = 0;
    if(abs(nx) < EPS) nx = 0;
 
    return make_pair(nx, ny);
}


//// https://atcoder.jp/contests/abc207/submissions/28082314
//// 重心を求める
pair<double, double> get_center(const vector<pair<double, double>>& X){
    long double gx = 0, gy = 0;
    for(auto& [x, y] : X){
        gx += x;
        gy += y;
    }
    gx /= X.size();
    gy /= X.size();
    return mp(gx, gy);
}

//// 点centerを中心に、aとbのなす角を返す. thetaは符号に注意?
long double get_theta(pair<double, double> a, pair<double, double> b, pair<double, double> center){
    a.first -= center.first, a.second -= center.second;
    b.first -= center.first, b.second -= center.second;
    long double cos_theta = a.first * b.first + a.second * b.second;
    cos_theta /= (1e-16 + sqrt(a.first * a.first + a.second * a.second));
    cos_theta /= (1e-16 + sqrt(b.first * b.first + b.second * b.second));
    return acos(cos_theta);
}

//// 点centerを中心にpointをtheta回転した点を返す
pair<double, double> rotate(pair<double, double> point, pair<double, double> center, double theta){
    point.first -= center.first, point.second -= center.second;
    point = pair<double, double>{point.first*cos(theta) - point.second*sin(theta), point.first*sin(theta) + point.second*cos(theta)};
    point.first += center.first, point.second += center.second;
    return point;
}
