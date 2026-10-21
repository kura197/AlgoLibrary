#pragma once

template <class T>
struct RangeAssignValue {
    T value;
    bool has_value;
};

template <class T>
RangeAssignValue<T> range_assign_composition(
    RangeAssignValue<T> f,
    RangeAssignValue<T> g
) {
    return f.has_value ? f : g;
}

template <class T>
RangeAssignValue<T> range_assign_id() {
    return {T{}, false};
}
