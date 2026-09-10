#pragma once
// https://github.com/kura197/AlgoLibrary/blob/main/library/randxor.h

#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

using namespace std;

namespace randxor_detail {

struct State {
    unsigned int x = 123456789;
    unsigned int y = 362436069;
    unsigned int z = 521288629;
    unsigned int w = 88675123;
};

State& state() {
    static State state;
    return state;
}

}  // namespace randxor_detail

// Set the seed. The same seed produces the same sequence.
void set_seed(unsigned int seed) {
    randxor_detail::State& state = randxor_detail::state();
    state.x = 123456789;
    state.y = 362436069;
    state.z = 521288629;
    state.w = seed;
}

// xor128
unsigned int randxor() {
    randxor_detail::State& state = randxor_detail::state();
    unsigned int t = state.x ^ (state.x << 11);
    state.x = state.y;
    state.y = state.z;
    state.z = state.w;
    state.w = (state.w ^ (state.w >> 19)) ^ (t ^ (t >> 8));
    return state.w;
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
