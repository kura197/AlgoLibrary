#pragma once

// https://bolero-fk.github.io/ProconLibrary/DataStructure/nth_accumulater.hpp

#include <array>
#include <cassert>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std;

// N 次元累積和
// 使い方:
// vector grid = {{1LL, 2LL}, {3LL, 4LL}};
// nthAccumulater<long long, 2> acc(grid);
// long long sum = acc.sum({0, 0}, {2, 2});        // 10
// long long sub = acc.sum({0, 1}, {2, 2});        // 6 (2 + 4) <-- [0, 2) for row, [1, 2) for col
// long long cyc = acc.cyclic_sum({-1, 0}, {1, 2}); // 周期的にみた区間和
//
// sum(l, r):
// 各次元について [l[i], r[i]) の半開区間の総和を返す
//
// cyclic_sum(l, r):
// 各次元を周期的につないだものとして [l[i], r[i]) の総和を返す
template <typename T, int DIMENSION_SIZE>
class nthAccumulater
{
public:
    static_assert(0 < DIMENSION_SIZE, "DIMENSION_SIZE must be positive.");

    using IntArray = array<int, DIMENSION_SIZE>;
    using LongArray = array<long long, DIMENSION_SIZE>;
    using Entry = pair<IntArray, T>;

private:
    vector<T> values;
    IntArray sizes;
    array<int, DIMENSION_SIZE + 1> sum_sizes;

    static IntArray to_int_array(initializer_list<int> a)
    {
        assert((int)a.size() == DIMENSION_SIZE);

        IntArray result{};
        int i = 0;

        for (const int x : a)
        {
            result[i++] = x;
        }

        return result;
    }

    static LongArray to_long_array(const IntArray &a)
    {
        LongArray result{};

        for (int i = 0; i < DIMENSION_SIZE; i++)
        {
            result[i] = a[i];
        }

        return result;
    }

    static LongArray to_long_array(initializer_list<long long> a)
    {
        assert((int)a.size() == DIMENSION_SIZE);

        LongArray result{};
        int i = 0;

        for (const long long x : a)
        {
            result[i++] = x;
        }

        return result;
    }

    void init()
    {
        int sum_size = 1;

        for (int i = 0; i < DIMENSION_SIZE; i++)
        {
            assert(0 < sizes[i]);

            sum_sizes[i] = sum_size;
            sum_size *= sizes[i] + 1; // 0 番目は累積和用
        }

        sum_sizes[DIMENSION_SIZE] = sum_size;

        assert(0 < sum_size);
        values.assign(sum_size, T{});
    }

    int get_index(const IntArray &indexes) const
    {
        int result = 0;

        for (int i = 0; i < DIMENSION_SIZE; i++)
        {
            assert(0 <= indexes[i] && indexes[i] <= sizes[i]);
            result += indexes[i] * sum_sizes[i];
        }

        return result;
    }

    void set(const IntArray &indexes, const T &value)
    {
        values[get_index(indexes)] = value;
    }

    // 外部から見た 0-indexed の座標に value を加算する。
    void add_value(const IntArray &indexes, const T &value)
    {
        IntArray shifted_indexes{};

        for (int i = 0; i < DIMENSION_SIZE; i++)
        {
            assert(0 <= indexes[i] && indexes[i] < sizes[i]);
            shifted_indexes[i] = indexes[i] + 1;
        }

        values[get_index(shifted_indexes)] += value;
    }

    void build()
    {
        const int total_size = sum_sizes[DIMENSION_SIZE];

        for (int dimension = 0; dimension < DIMENSION_SIZE; dimension++)
        {
            const int stride = sum_sizes[dimension];
            const int width = sizes[dimension] + 1;
            const int block_size = stride * width;

            for (int block = 0; block < total_size; block += block_size)
            {
                for (int offset = 0; offset < stride; offset++)
                {
                    for (int k = 1; k <= sizes[dimension]; k++)
                    {
                        const int now = block + k * stride + offset;
                        values[now] += values[now - stride];
                    }
                }
            }
        }
    }

    T sum_from_origin(const unsigned int bit_mask, const LongArray &indexes) const
    {
        T result = T{};
        unsigned int sub_bit = bit_mask;

        do
        {
            T count = T{1};
            IntArray query_index{};

            for (int i = 0; i < DIMENSION_SIZE; i++)
            {
                if ((sub_bit >> i) & 1u)
                {
                    count *= indexes[i] / sizes[i];
                    query_index[i] = sizes[i];
                }
                else
                {
                    query_index[i] = static_cast<int>(indexes[i] % sizes[i]);
                }
            }

            result += count * values[get_index(query_index)];
            sub_bit = (sub_bit - 1) & bit_mask;
        } while (sub_bit != bit_mask);

        return result;
    }

    template <typename V>
    void set_sizes(const V &v, const int depth = 0)
    {
        if constexpr (is_arithmetic_v<V>)
        {
            assert(depth == DIMENSION_SIZE);
            return;
        }
        else
        {
            assert(depth < DIMENSION_SIZE);
            assert(!v.empty());

            sizes[depth] = static_cast<int>(v.size());
            set_sizes(v[0], depth + 1);
        }
    }

    template <typename V>
    void set_values(const V &v, IntArray &index, const int depth = 0)
    {
        if constexpr (is_arithmetic_v<V>)
        {
            set(index, v);
        }
        else
        {
            assert((int)v.size() == sizes[depth]);

            for (int i = 0; i < (int)v.size(); i++)
            {
                index[depth] = i + 1;
                set_values(v[i], index, depth + 1);
            }
        }
    }

    template <typename L, typename R>
    T sum_impl(const L &l, const R &r) const
    {
        for (int i = 0; i < DIMENSION_SIZE; i++)
        {
            assert(0 <= l[i] && l[i] < r[i] && r[i] <= sizes[i]);
        }

        if constexpr (DIMENSION_SIZE == 1)
        {
            return values[r[0]] - values[l[0]];
        }
        else if constexpr (DIMENSION_SIZE == 2)
        {
            const int s0 = sum_sizes[0];
            const int s1 = sum_sizes[1];

            return values[r[0] * s0 + r[1] * s1] -
                   values[l[0] * s0 + r[1] * s1] -
                   values[r[0] * s0 + l[1] * s1] +
                   values[l[0] * s0 + l[1] * s1];
        }
        else if constexpr (DIMENSION_SIZE == 3)
        {
            const int s0 = sum_sizes[0];
            const int s1 = sum_sizes[1];
            const int s2 = sum_sizes[2];

            auto index = [&](const auto x, const auto y, const auto z)
            {
                return x * s0 + y * s1 + z * s2;
            };

            return values[index(r[0], r[1], r[2])] -
                   values[index(l[0], r[1], r[2])] -
                   values[index(r[0], l[1], r[2])] -
                   values[index(r[0], r[1], l[2])] +
                   values[index(l[0], l[1], r[2])] +
                   values[index(l[0], r[1], l[2])] +
                   values[index(r[0], l[1], l[2])] -
                   values[index(l[0], l[1], l[2])];
        }
        else
        {
            T result = T{};

            for (unsigned int bit = 0; bit < (1u << DIMENSION_SIZE); bit++)
            {
                long long index = 0;

                for (int i = 0; i < DIMENSION_SIZE; i++)
                {
                    index += (((bit >> i) & 1u) ? r[i] : l[i]) * sum_sizes[i];
                }

                if ((__builtin_popcount(bit) % 2) == (DIMENSION_SIZE % 2))
                {
                    result += values[index];
                }
                else
                {
                    result -= values[index];
                }
            }

            return result;
        }
    }

    T cyclic_sum_impl(LongArray l, LongArray r) const
    {
        for (int i = 0; i < DIMENSION_SIZE; i++)
        {
            assert(l[i] < r[i]);
        }

        // l を非負に寄せる。
        for (int i = 0; i < DIMENSION_SIZE; i++)
        {
            if (l[i] < 0)
            {
                const long long mod = sizes[i];
                const long long next_l = (l[i] % mod + mod) % mod;

                r[i] += next_l - l[i];
                l[i] = next_l;
            }
        }

        T result = T{};

        for (unsigned int bit = 0; bit < (1u << DIMENSION_SIZE); bit++)
        {
            LongArray indexes{};
            unsigned int bit_mask = 0;

            for (int i = 0; i < DIMENSION_SIZE; i++)
            {
                indexes[i] = ((bit >> i) & 1u) ? r[i] : l[i];

                if (sizes[i] <= indexes[i])
                {
                    bit_mask |= (1u << i);
                }
            }

            if ((__builtin_popcount(bit) % 2) == (DIMENSION_SIZE % 2))
            {
                result += sum_from_origin(bit_mask, indexes);
            }
            else
            {
                result -= sum_from_origin(bit_mask, indexes);
            }
        }

        return result;
    }

public:
    template <typename V>
    nthAccumulater(const vector<V> &v)
    {
        set_sizes(v);

        init();

        IntArray index{};
        set_values(v, index);

        build();
    }

    // 各次元の大きさと {index, value} の列から構築する。同じ index は加算する。
    nthAccumulater(const IntArray &sizes_, const vector<Entry> &entries)
        : sizes(sizes_)
    {
        init();

        for (const auto &[indexes, value] : entries)
        {
            add_value(indexes, value);
        }

        build();
    }

    nthAccumulater(initializer_list<int> sizes_, const vector<Entry> &entries)
        : nthAccumulater(to_int_array(sizes_), entries)
    {
    }

    T sum(const IntArray &l, const IntArray &r) const
    {
        return sum_impl(l, r);
    }

    T sum(const LongArray &l, const LongArray &r) const
    {
        return sum_impl(l, r);
    }

    T sum(initializer_list<long long> l, initializer_list<long long> r) const
    {
        assert((int)l.size() == DIMENSION_SIZE);
        assert((int)r.size() == DIMENSION_SIZE);

        return sum_impl(l.begin(), r.begin());
    }

    T cyclic_sum(const IntArray &l, const IntArray &r) const
    {
        return cyclic_sum_impl(to_long_array(l), to_long_array(r));
    }

    T cyclic_sum(const LongArray &l, const LongArray &r) const
    {
        return cyclic_sum_impl(l, r);
    }

    T cyclic_sum(initializer_list<long long> l, initializer_list<long long> r) const
    {
        return cyclic_sum_impl(to_long_array(l), to_long_array(r));
    }
};
