//
// Created by sayea on 5/20/2023.
//

#pragma once
#include <compare>

#include "array.hpp"
#include "inc.hpp"
#include "iterator"
#include "vector.hpp"

namespace my {

template<typename T, usize N>
struct Range : public Iterator<Range<T, N>> {
private:
    template<typename U, usize S>
    struct Span {
        U *itr{nullptr};

        constexpr Span() = default;

        template<typename I, usize P>
        constexpr Span(const I *i) {
            itr = new I[P];
            copy(i, P, itr);
        }

        template<typename V, usize P>
        constexpr Span(const Span<V, P> &rhs) {
            itr = new V[P];
            copy(rhs.itr, P, itr);
        }

        template<typename V, usize P>
        constexpr Span(Span<V, P> &&rhs) {
            itr = exchange(rhs.itr, nullptr);
        }

        template<typename I, usize P>
        constexpr Span(I (&a)[P]) {
            itr = new I[P];
            copy(a, P, itr);
        }

        constexpr ~Span() noexcept { delete[] itr; }

        constexpr auto size() const -> usize { return N; }

        constexpr auto front() -> T & { return *itr; }

        constexpr auto back() -> T & { return itr[N - 1]; }

        constexpr auto operator[](usize idx) -> T & { return itr[idx]; }

        constexpr auto operator[](const usize idx) const -> const T & { return itr[idx]; }

        constexpr auto begin() -> T * { return itr; }

        constexpr auto end() -> T * { return itr + N; }

        constexpr auto begin() const -> const T * { return itr; }

        constexpr auto end() const -> const T * { return itr + N; }
    };

    template<typename U>
    struct Span<U, dyn_ext> {
        U *itr{nullptr};
        U *sen{nullptr};

        constexpr Span() = default;

        template<typename V>
        constexpr Span(const Span<V, dyn_ext> &rhs) {
            usize nsz = rhs.size();
            itr = new V[nsz];
            copy(rhs.itr, rhs.sen, itr);
            sen = itr + nsz;
        }

        template<typename V>
        constexpr Span(Span<V, dyn_ext> &&rhs) {
            itr = move(rhs.itr);
            sen = move(rhs.sen);
        }

        template<typename I>
        constexpr Span(const I *i, const usize sz) {
            itr = new I[sz];
            copy(i, sz, itr);
            sen = itr + sz;
        }

        template<typename I>
        constexpr Span(const I *first, const I *last) {
            usize nsz = last - first;
            itr = new I[nsz];
            copy(first, last, itr);
            sen = itr + nsz;
        }

        constexpr ~Span() noexcept { delete[] itr; }

        constexpr auto size() const -> usize { return sen - itr; }

        constexpr auto front() -> T & { return *itr; }

        constexpr auto back() -> T & { return *(sen - 1); }

        constexpr auto operator[](usize idx) -> T & { return itr[idx]; }

        constexpr auto operator[](const usize idx) const -> const T & { return itr[idx]; }

        constexpr auto begin() -> T * { return itr; }

        constexpr auto end() -> T * { return sen; }

        constexpr auto begin() const -> const T * { return itr; }

        constexpr auto end() const -> const T * { return sen; }
    };

    Span<T, N> span{};

public:
    constexpr Range() = default;

    constexpr Range(const Range &rhs) : span{rhs.span} {}

    constexpr auto operator=(const Range &rhs) -> Range & {
        if (this != &rhs) span = rhs.span;
        return *this;
    }

    constexpr Range(Range &&rhs) noexcept : span{move(rhs.span)} {}

    constexpr auto operator=(Range &&rhs) noexcept -> Range & {
        if (this != &rhs) span = move(rhs.span);
        return *this;
    }

    template<typename I>
    constexpr Range(const I *i, const usize sz) : span{i, sz} {}

    template<typename I>
    constexpr Range(I first, I last) : span{first, last} {}

    template<typename I, usize S>
    constexpr Range(I (&a)[S]) : span{a} {}

    constexpr Range(Array<T, N> &a) : span{a.begin()} {}

    constexpr Range(const Array<T, N> &a) : span{a.begin()} {}

    constexpr Range(Vector<T> &v) : span{v.begin(), v.end()} {}

    constexpr Range(const Vector<T> &v) : span{v.begin(), v.end()} {}

    constexpr bool operator<=>(const Range &) const = default;

    constexpr ~Range() noexcept = default;

    constexpr auto size() const -> usize { return span.size(); }

    constexpr auto front() -> T & { return span.front(); }

    constexpr auto back() -> T & { return span.back(); }

    constexpr auto operator[](usize idx) -> T & { return span[idx]; }

    constexpr auto operator[](const usize idx) const -> const T & { return span[idx]; }

    constexpr auto begin() -> T * { return span.begin(); }

    constexpr auto end() -> T * { return span.end(); }

    constexpr auto begin() const -> const T * { return span.begin(); }

    constexpr auto end() const -> const T * { return span.end(); }
};

template<typename I, typename E>
Range(I, E) -> Range<I, dyn_ext>;

template<typename T, usize N>
Range(T (&)[N]) -> Range<T, N>;

template<typename T, usize N>
Range(Array<T, N> &) -> Range<T, N>;

template<typename T, usize N>
Range(const Array<T, N> &) -> Range<const T, N>;

template<typename T>
Range(Vector<T> &) -> Range<T, dyn_ext>;

template<typename T>
Range(const Vector<T> &) -> Range<const T, dyn_ext>;

}  // namespace my

my::Range<char, my::dyn_ext> operator""_ma(const char *ch, const my::usize len) {
    return my::Range<char, my::dyn_ext>{(char *) ch, len};
}

my::Range<char, 32> operator""_ms(const char *ch, const my::usize len) {
    return my::Range<char,32>{ch, len};
}
