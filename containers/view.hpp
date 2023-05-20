#pragma once
#include <compare>
#include <utility>

#include "array.hpp"
#include "inc.hpp"
#include "iterator"
#include "vector.hpp"

namespace my {

template<typename T, usize N>
struct View : public Iterator<View<T, N>> {
private:
    template<typename U, usize S>
    struct Span {
        U *itr{nullptr};
    };
    template<typename U>
    struct Span<U, dyn_ext> {
        U *itr{nullptr};
        U *sen{nullptr};
    };

    Span<T, N> span{};

  public:
    constexpr View() = default;

     constexpr View(const View &) = default;

     constexpr auto operator=(const View &) -> View & = default;

     constexpr View(View &&) noexcept = default;

     constexpr auto operator=(View &&) noexcept -> View & = default;

    template<typename I>
    constexpr View(I i, const usize sz) : span{i} {
        if constexpr (N == -1ULL) span.sen = span.itr + sz;
    }

    template<typename I>
    constexpr View(I first, I last) : span{first} {
        if constexpr (N == -1ULL) span.sen = last;
    }

    template<usize sz>
    constexpr View(T (&a)[sz]) : span{a} {}

    constexpr View(Array<T, N> &a) : span{a.begin()} {}

    constexpr View(const Array<T, N> &a) : span{a.begin()} {}

    constexpr View(Vector<T> &v) : span{v.begin(), v.end()} {}

    constexpr View(const Vector<T> &v) : span{v.begin(), v.end()} {}

    template<typename U>
    constexpr View(const View<U, N> &rhs) : span{rhs.begin()} {}

    // template<typename U>
    // constexpr View(const View<U, dyn_ext> &rhs) : span{rhs.begin(), rhs.end()} {}

    constexpr bool operator<=>(const View &) const = default;

    constexpr ~View() = default;

    constexpr auto size() const -> usize {
        if constexpr (N == -1ULL) return span.sen - span.itr;
        return N;
    }

    constexpr auto begin() -> T * { return span.itr; }

    constexpr auto begin() const -> T * { return span.itr; }

    constexpr auto end() -> T * {
        if constexpr (N == -1ULL) return span.sen;
        return span.itr + N;
    }

    constexpr auto end() const -> T * {
        if constexpr (N == -1ULL) return span.sen;
        return span.itr + N;
    }

    constexpr auto operator[](const usize idx) -> T & {
        return span.itr[idx];
    }

    constexpr auto operator[](const usize idx) const -> T & {
        return span.itr[idx];
    }
};

template<typename I, typename E>
View(I, E) -> View<I, dyn_ext>;

template<typename T, usize N>
View(T (&)[N]) -> View<T, N>;

template<typename T, usize N>
View(Array<T, N> &) -> View<T, N>;

template<typename T, usize N>
View(const Array<T, N> &) -> View<const T, N>;

template<typename T>
View(Vector<T> &) -> View<T, dyn_ext>;

template<typename T>
View(const Vector<T> &) -> View<const T, dyn_ext>;

}  // namespace my

my::View<char, my::dyn_ext> operator""_mv(const char *ch, const my::usize len) {
    return my::View<char, my::dyn_ext>{(char *) ch, len};
}
