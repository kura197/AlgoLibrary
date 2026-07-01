#include <algorithm>
#include <vector>

// 全点対最短路を計算する O(V^3)
// d[i][j] := 頂点iから頂点jへの距離
// 到達不能な距離は INF とし、INF + x がオーバーフローしない値を使う
void warshall_floyd(std::vector<std::vector<long long>>& d, long long INF) {
    int vertex_count = (int)d.size();
    for (int k = 0; k < vertex_count; k++) {
        for (int i = 0; i < vertex_count; i++) {
            if (d[i][k] == INF) continue;
            for (int j = 0; j < vertex_count; j++) {
                if (d[k][j] == INF) continue;
                d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
            }
        }
    }
}
