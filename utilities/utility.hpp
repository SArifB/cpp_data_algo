//
// Created by sayea on 5/20/2023.
//

#pragma once

namespace my {

using usize = size_t;
inline constexpr usize dyn_ext = static_cast<usize>(-1);

template<typename T> struct RmRef {
    using type = T;
};
template<typename T> struct RmRef<T &> {
    using type = T;
};
template<typename T> struct RmRef<T &&> {
    using type = T;
};
template<typename T> using RmRef_tp = typename RmRef<T>::type;

template<typename T> struct RmPtr {
    using type = T;
};
template<typename T> struct RmPtr<T *> {
    using type = T;
};
template<typename T> struct RmPtr<T *const> {
    using type = T;
};
template<typename T> struct RmPtr<T *volatile> {
    using type = T;
};
template<typename T> struct RmPtr<T *const volatile> {
    using type = T;
};
template<typename T> using RmPtr_tp = typename RmPtr<T>::type;

template<typename T>
inline constexpr auto move(T &&arg) noexcept -> RmRef_tp<T> && { return static_cast<RmRef_tp<T> &&>(arg); }

template<typename T>
inline constexpr auto forward(RmRef_tp<T> &arg) noexcept -> T && { return static_cast<T &&>(arg); }

template<typename T>
inline constexpr auto forward(RmRef_tp<T> &&arg) noexcept -> T && { return static_cast<T &&>(arg); }

template<typename T, typename U = T>
inline constexpr auto exchange(T &old_val, U &&new_val) noexcept -> T {
    T tmp = move(old_val);
    old_val = move(new_val);
    return tmp;
}

template<typename T>
inline constexpr auto swap(T &left_val, T &right_val) noexcept -> void {
    T tmp = move(left_val);
    left_val = move(right_val);
    right_val = move(tmp);
}

template<typename T>
inline constexpr auto swap(T *left_val, T *right_val) noexcept -> void {
    val_swap(*left_val, *right_val);
}

template<typename T, usize size>
inline constexpr auto swap(T (&left_arr)[size], T (&right_arr)[size]) noexcept -> void {
    if (&left_arr != &right_arr)
        for (T *tmp1{left_arr}, *tmp2{right_arr}; tmp1 != (tmp1 + size); ++tmp1, ++tmp2)
            itr_swap(tmp1, tmp2);
}

template<typename T>
inline constexpr auto copy(const T *begin, const T *const end, T *out) noexcept -> T * {
    for (; begin != end; ++begin, ++out)
        *out = *begin;
    return out;
}

template<typename T>
inline constexpr auto copy(const T *begin, usize size, T *out) noexcept -> T * {
    for (usize i{}; i < size; ++i)
        out[i] = begin[i];
    return out;
}

template<typename T>
inline constexpr auto find(const T *begin, const T *const end, const T &val) noexcept -> T * {
    for (; begin != end; ++begin)
        if (*begin == val) return begin;
    return end;
}

}  // namespace my
