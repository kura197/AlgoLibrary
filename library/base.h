#pragma once

#include <algorithm>
#include <cassert>
#include <string>

using namespace std;

long long baseX_to_ll(const string& s, int base) {
    assert(2 <= base && base < 10);
    long long ret = 0;
    for (char c : s) {
        ret = ret * base + (c - '0');
    }
    return ret;
}

string ll_to_baseX(long long x, int base) {
    assert(2 <= base && base < 10);
    if (x == 0) return "0";

    string ret;
    while (x > 0) {
        ret += char('0' + (x % base));
        x /= base;
    }
    reverse(ret.begin(), ret.end());
    return ret;
}
