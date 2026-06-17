// check by tenkei-009: https://atcoder.jp/contests/typical90/submissions/76743434

struct point {
    int x, y;
};

long long cross(const point &a, const point &b) {
    return (long long) a.x * b.y - (long long) a.y * b.x;
}

long long dot(const point &a, const point &b) {
    return (long long) a.x * b.x + (long long) a.y * b.y;
}

bool cmp(const point &a, const point &b) {
    int ah = (a.y < 0 or (a.y == 0 and a.x < 0));
    int bh = (b.y < 0 or (b.y == 0 and b.x < 0));
    if (ah != bh) return ah < bh;
    return cross(a, b) > 0;
}

// need to remove origin point ((0, 0))
void argument_sort(vector<point> &points) {
    sort(points.begin(), points.end(), cmp);
}

// return angle a to b
double get_radian(point a, point b) {
    double rad = atan2(cross(a, b), dot(a, b)); // [-pi, pi]
    return rad;
}

double get_degree(point a, point b) {
    return get_radian(a, b) * 180.0 / acos(-1); // [-180, 180]
}
