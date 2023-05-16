#pragma once
#include <compare>
#include <utility>

#include "cont.hpp"
#include "array.hpp"
#include "iterator"
#include "vector.hpp"

namespace my {

template<typename T, usize N>
struct Span : public Iterator<Span<T, N>> {
private:
    template<typename U, usize S>
    struct BaseRng {
        U *itr{nullptr};
    };
    template<typename U>
    struct BaseRng<U, -1ULL> {
        U *itr{nullptr};
        U *sen{nullptr};
    };

    BaseRng<T, N> rng{};

public:
    constexpr Span() = default;

    constexpr Span(const Span &) = default;

    constexpr auto operator=(const Span &) -> Span & = default;

    constexpr Span(Span &&) noexcept = default;

    constexpr auto operator=(Span &&) noexcept -> Span & = default;

    template<typename I>
    constexpr Span(I i, const usize sz) : rng{i} {
        if constexpr (N == -1ULL) rng.sen = rng.itr + sz;
    }

    template<typename I>
    constexpr Span(I first, I last) : rng{first} {
        //    [[maybe_unused]] auto dist = first - last;
        //    static_assert(dist >= 0, "Invalid range");
        //    static_assert(dist == N, "Invalid range");
        if constexpr (N == -1ULL) rng.sen = last;
    }

    template<usize sz>
    constexpr Span(T (&a)[sz]) : rng{a} {}
    //    if constexpr (N == -1ULL) this->sen = itr + sz;


//    template<typename U>
    constexpr Span(Array<T, N> &a) : rng{a.begin()} {}
    //    if constexpr (N == -1ULL) this->sen = a.end();

//    template<typename U>
    constexpr Span(const Array<T, N> &a) : rng{a.begin()} {}
    //    if constexpr (N == -1ULL) this->sen = a.end();

//    template<typename U>
    constexpr Span(Vector<T> &v) : rng{v.begin(), v.end()} {}
    // if constexpr (N == -1ULL)
    // rng.sen = v.end();

//    template<typename U>
    constexpr Span(const Vector<T> &v) : rng{v.begin(), v.end()} {}
    // if constexpr (N == -1ULL)
    // rng.sen = v.end();
    //
    //  template<typename U>
    //  constexpr Span(const Span<U, N> &rhs)
    //      : itr{rhs.begin()} {
    //    if constexpr (N == -1ULL) this->sen = rhs.end();
    //  }

    template<typename U>
    constexpr Span(const Span<U, -1ULL> &rhs) : rng{rhs.begin()} {
        static_assert(N == rhs.size(), "Size mismatch");
        if constexpr (N == -1ULL) rng.sen = rhs.end();
    }

    constexpr auto operator<=>(const Span &) const -> bool = default;

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
        static_assert(idx < N, "Size mismatch");
        return rng.itr[idx];
    }

    constexpr auto operator[](const usize idx) const -> T & {
        static_assert(idx < N, "Size mismatch");
        return rng.itr[idx];
    }
};

template<typename I, typename E>
Span(I, E) -> Span<I, -1ULL>;

template<typename T, usize N>
Span(T (&)[N]) -> Span<T, N>;

 template <typename T, usize N> Span(Array<T, N> &) -> Span<T, N>;

 template <typename T, usize N>
 Span(const Array<T, N> &) -> Span<const T, N>;

 template <typename T> Span(Vector<T> &) -> Span<T, -1ULL>;

 template <typename T> Span(const Vector<T> &) -> Span<const T, -1ULL>;

}  // namespace my

my::Span<char, -1ULL> operator ""_mv(const char *ch, const usize len) {
    return my::Span<char, -1ULL>{(char *) ch, len};
}

// Other

/*
 *
struct E { };
[[no_unique_address]] std::conditional_t<some_condition(), bool, E> newField;
 * //
//    template<typename I>
//    constexpr PtrArray(I i, usize sz)  : itr{std::to_address(i)} {
////      (void) sz;
//      static_assert(sz == N, "Size mismatch");
//    }
//
//    template<typename I>
//    constexpr PtrArray(I first, I last)  :
//    itr{std::to_address(first)} {
//      [[maybe_unused]] auto dist = first - last;
//      static_assert(dist >= 0, "Invalid range");
//      static_assert(dist == N, "Invalid range");
//    }


//  template<class I, class E> PtrArray(I, E) ->
PtrArray<std::remove_reference_t<std::iter_reference_t<I>>>;
//  template<class R> PtrArray(R&&) ->
PtrArray<std::remove_reference_t<std::ranges::range_reference_t<R>>>;
 *
 *
//    template<typename R>
//    constexpr PtrArray(R &&r)  :itr{r.data()} { static_assert(r.  N()
== N, "Size mismatch"); }

//    template<typename U>
//    constexpr PtrArray(const PtrArray<U, -1LLU> &other)  :
itr{other.begin()} { static_assert(other.N() == N, "Size mismatch"); }

    template<typename F>
    constexpr auto for_each(const F &&f)   -> void {
      for (auto i{begin()}, k{end()}; i != k; ++i) f(*i);
    }

    template<usize bn, usize nd, typename F>
    constexpr auto for_each(const F &&f)   -> void {
      for (auto i{begin() + bn}, k{begin() + nd}; i != k; ++i) f(*i);
    }

    template<typename OS>
    friend auto operator<<(OS &os, PtrArray<T, N> &rhs)   -> OS & {
      os << '[' << ' ';
      for (usize i{}; i != N; ++i) os << rhs.itr[i] << ", ";
      return os << ']';
    }

    template<typename OS>
    friend auto operator<<(OS &os, const PtrArray<T, N> &rhs)   -> OS & {
      os << '[' << ' ';
      for (usize i{}; i != N; ++i) os << rhs.itr[i] << ", ";
      return os << ']';
    }
    */
