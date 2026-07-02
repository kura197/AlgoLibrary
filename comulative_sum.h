#pragma once

#include <vector>

using namespace std;

/// 2次元累積和
/// row-first

template<typename T>
struct CumulativeSum{
    vector<vector<T>> sum;

    /// array[row][col]
    CumulativeSum(const vector<vector<T>>& array){
        int row = array.size();
        int col = array[0].size();
        sum.resize(row, vector<T>(col, 0));

        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                sum[i][j] = array[i][j];
                if(i > 0)
                    sum[i][j] += sum[i-1][j];
                if(j > 0)
                    sum[i][j] += sum[i][j-1];
                if(i > 0 && j > 0)
                    sum[i][j] -= sum[i-1][j-1];
            }
        }
    }

    //// return sum of [x1, x2], [y1, y2]
    T get_val(int x1, int y1, int x2, int y2){
        T ret = sum[y2][x2];
        if(y1 > 0)
            ret -= sum[y1-1][x2];
        if(x1 > 0)
            ret -= sum[y2][x1-1];
        if(y1 > 0 && x1 > 0)
            ret += sum[y1-1][x1-1];
        return ret;
    }
};
