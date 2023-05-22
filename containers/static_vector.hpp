//
// Created by sayea on 5/20/2023.
//

#pragma once
#include "view.hpp"
#include <compare>

namespace my {

template<typename T, usize N = 16>
struct SttVec {
    Array<T, N> data{};
    usize len{};

    constexpr SttVec() = default;
    constexpr ~SttVec() = default;
    constexpr SttVec(const SttVec &) = default;
    constexpr auto operator=(const SttVec &) -> SttVec & = default;
    constexpr SttVec(SttVec &&) noexcept = default;
    constexpr auto operator=(SttVec &&) noexcept -> SttVec & = default;

    //    constexpr SttVec(const SttVec &rhs) : data{rhs.data}, len{rhs.len} {}
    //    constexpr SttVec(SttVec &&rhs) : data{move(rhs.data)}, len{move(rhs.len)} {}

    constexpr SttVec(const View<T, -1ULL> &ptr) : data{ptr.begin(), ptr.end()}, len{ptr.size()} {}
    constexpr SttVec(const T *ptr, usize size) : data{ptr, ptr + size}, len{size} {}

    constexpr bool operator<=>(const SttVec &) const = default;

    constexpr auto operator[](usize idx) -> T & { return data[idx]; }
    constexpr auto operator[](usize idx) const -> const T & { return data[idx]; }

    constexpr auto push_back(T &&val) -> void { data[len++] = move(val); }
    constexpr auto push_back(const T &val) -> void { data[len++] = val; }

    constexpr auto size() -> usize { return len; }
    constexpr auto begin() -> T * { return data.begin(); }
    constexpr auto begin() const -> const T * { return data.begin(); }
    constexpr auto end() -> T * { return data.begin() + len; }
    constexpr auto end() const -> const T * { return data.begin() + len; }
};

template<typename T, usize N>
SttVec(T (&)[N]) -> SttVec<T, N>;

template<template<typename> typename I, typename T>
SttVec(I<T> first, I<T> last) -> SttVec<I<T>>;

}  // namespace my

constexpr auto operator""_w(const char *str, my::usize size) -> my::SttVec<char, 16> {
    return my::SttVec<char, 16>{(char *) str, size - 1};
};
