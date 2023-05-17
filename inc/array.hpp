#pragma once
#include <compare>

#include "cont.hpp"
#include "iterator.hpp"

namespace my {

template<typename T, usize N = 0>
struct Array : public Iterator<Array<T, N>> {
  private:
    T buf[N]{};

  public:
    constexpr Array() = default;

    constexpr Array(const auto &...args) : buf{args...} {}

    constexpr Array(const Array &) = default;

    constexpr auto operator=(const Array &) -> Array & = default;

    constexpr Array(Array &&) = default;

    constexpr auto operator=(Array &&) -> Array & = default;

    constexpr auto operator<=>(const Array &) const -> bool = default;

    constexpr ~Array() = default;

    constexpr auto size() const -> usize;

    constexpr auto front() -> T & { return buf[0]; }

    constexpr auto back() -> T & { return buf[N - 1]; }

    constexpr auto begin() -> T * { return buf; }

    constexpr auto end() -> T * { return buf + N; }

    constexpr auto operator[](usize idx) -> T & { return buf[idx]; }

    constexpr auto begin() const -> const T * { return buf; }

    constexpr auto end() const -> const T * { return buf + N; }

    constexpr auto operator[](const usize idx) const -> const T & {
        return buf[idx];
    }
};

template<typename T, usize N>
constexpr auto Array<T, N>::size() const -> usize { return N; }

template<typename T, typename... U>
Array(T, U...) -> Array<T, 1 + sizeof...(U)>;

}  // namespace my
