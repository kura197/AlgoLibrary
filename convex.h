using P = pair<ll, ll>;

ll cross(const P& a, const P& b, const P& c) {
    // ベクトル ab と ac の外積
    // 正: a->b->c が反時計回り
    // 負: 時計回り
    // 0 : 一直線上
    return (b.first - a.first) * (c.second - a.second)
         - (b.second - a.second) * (c.first - a.first);
}

// 凸包を返す
// include_collinear = false: 辺上の点を含めない
// include_collinear = true : 辺上の点も含める
vector<P> convex_hull(vector<P> ps, bool include_collinear = false) {
    sort(ps.begin(), ps.end());
    ps.erase(unique(ps.begin(), ps.end()), ps.end());

    int n = ps.size();
    if (n <= 1) return ps;

    vector<P> lower, upper;

    for (auto p : ps) {
        while (lower.size() >= 2) {
            ll cr = cross(lower[lower.size() - 2], lower[lower.size() - 1], p);

            if (include_collinear) {
                if (cr < 0) lower.pop_back();
                else break;
            } else {
                if (cr <= 0) lower.pop_back();
                else break;
            }
        }
        lower.push_back(p);
    }

    for (int i = n - 1; i >= 0; i--) {
        P p = ps[i];
        while (upper.size() >= 2) {
            ll cr = cross(upper[upper.size() - 2], upper[upper.size() - 1], p);

            if (include_collinear) {
                if (cr < 0) upper.pop_back();
                else break;
            } else {
                if (cr <= 0) upper.pop_back();
                else break;
            }
        }
        upper.push_back(p);
    }

    // lower の末尾と upper の末尾は重複するので除く
    lower.pop_back();
    upper.pop_back();

    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}