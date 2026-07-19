#pragma once

#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

using namespace std;

// xor128
unsigned int randxor() {
    static unsigned int x = 123456789;
    static unsigned int y = 362436069;
    static unsigned int z = 521288629;
    static unsigned int w = 88675123;
    unsigned int t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    return w;
}

// return [0, 1)
double rand01() {
    return 1.0 * randxor() / numeric_limits<unsigned int>::max();
}

// return [left, right)
int rand_int(int left, int right) {
    assert(right > left);
    return (int)(randxor() % (unsigned int)(right - left)) + left;
}

template<class T>
void shuffle(vector<T>& values) {
    int size = (int)values.size();
    for (int i = size; i > 1; i--) {
        int p = rand_int(0, i);
        swap(values[i - 1], values[p]);
    }
}

template<class T>
T sample(const vector<T>& values) {
    assert(!values.empty());
    return values[rand_int(0, (int)values.size())];
}
