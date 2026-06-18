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


////////////////////////

struct point3 {
    long long x, y, z;
};

point3 operator+(const point3& a, const point3& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

point3 operator-(const point3& a, const point3& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

long long dot(const point3& a, const point3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

point3 cross(const point3& a, const point3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

long long norm2(const point3& a) {
    return dot(a, a);
}

double norm(const point3& a) {
    return sqrt((double)norm2(a));
}

// 2ベクトルのなす角 [0, pi]
double get_radian(const point3& a, const point3& b) {
    return atan2(norm(cross(a, b)), (double)dot(a, b));
}

double get_degree(const point3& a, const point3& b) {
    return get_radian(a, b) * 180.0 / acos(-1);
}

// 角ABC [0, pi]
double angle_radian(const point3& A, const point3& B, const point3& C) {
    return get_radian(A - B, C - B);
}

double angle_degree(const point3& A, const point3& B, const point3& C) {
    return angle_radian(A, B, C) * 180.0 / acos(-1);
}

// normal方向から見た符号付き角 [-pi, pi]
double get_signed_radian(const point3& a, const point3& b, const point3& normal) {
    return atan2((double)dot(normal, cross(a, b)), (double)dot(a, b));
}

double get_signed_degree(const point3& a, const point3& b, const point3& normal) {
    return get_signed_radian(a, b, normal) * 180.0 / acos(-1);
}