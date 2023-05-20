#pragma once
#include <compare>
#include <utility>

#include "array.hpp"
#include "cont.hpp"
#include "iterator"
#include "vector.hpp"

namespace my {

inline constexpr usize dyn_ext = static_cast<usize>(-1);

template<typename T, usize N>
struct Span : public Iterator<Span<T, N>> {
private:
    template<typename U, usize S>
    struct BaseRng {
        U *itr{nullptr};
    };
    template<typename U>
    struct BaseRng<U, dyn_ext> {
        U *itr{nullptr};
        U *sen{nullptr};
    };

    BaseRng<T, N> rng{};

  public:
    constexpr Span() = default;

    // constexpr Span(const Span &) = default;

    // constexpr auto operator=(const Span &) -> Span & = default;

    // constexpr Span(Span &&) noexcept = default;

    // constexpr auto operator=(Span &&) noexcept -> Span & = default;

    template<typename I>
    constexpr Span(I i, const usize sz) : rng{i} {
        if constexpr (N == -1ULL) rng.sen = rng.itr + sz;
    }

    template<typename I>
    constexpr Span(I first, I last) : rng{first} {
        if constexpr (N == -1ULL) rng.sen = last;
    }

    template<usize sz>
    constexpr Span(T (&a)[sz]) : rng{a} {}

    constexpr Span(Array<T, N> &a) : rng{a.begin()} {}

    constexpr Span(const Array<T, N> &a) : rng{a.begin()} {}

    constexpr Span(Vector<T> &v) : rng{v.begin(), v.end()} {}

    constexpr Span(const Vector<T> &v) : rng{v.begin(), v.end()} {}

    template<typename U>
    constexpr Span(const Span<U, N> &rhs) : rng{rhs.begin()} {}

    // template<typename U>
    // constexpr Span(const Span<U, dyn_ext> &rhs) : rng{rhs.begin(), rhs.end()} {}

    constexpr bool operator<=>(const Span &) const = default;

    constexpr ~Span() = default;

    constexpr auto size() const -> usize {
        if constexpr (N == -1ULL) return rng.sen - rng.itr;
        return N;
    }

    constexpr auto begin() -> T * { return rng.itr; }

    constexpr auto begin() const -> T * { return rng.itr; }

    constexpr auto end() -> T * {
        if constexpr (N == -1ULL) return rng.sen;
        return rng.itr + N;
    }

    constexpr auto end() const -> T * {
        if constexpr (N == -1ULL) return rng.sen;
        return rng.itr + N;
    }

    constexpr auto operator[](const usize idx) -> T & {
        return rng.itr[idx];
    }

    constexpr auto operator[](const usize idx) const -> T & {
        return rng.itr[idx];
    }
};

template<typename I, typename E>
Span(I, E) -> Span<I, dyn_ext>;

template<typename T, usize N>
Span(T (&)[N]) -> Span<T, N>;

template<typename T, usize N>
Span(Array<T, N> &) -> Span<T, N>;

template<typename T, usize N>
Span(const Array<T, N> &) -> Span<const T, N>;

template<typename T>
Span(Vector<T> &) -> Span<T, dyn_ext>;

template<typename T>
Span(const Vector<T> &) -> Span<const T, dyn_ext>;

}  // namespace my

my::Span<char, my::dyn_ext> operator""_mv(const char *ch, const usize len) {
    return my::Span<char, my::dyn_ext>{(char *) ch, len};
}
