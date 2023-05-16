#pragma once
#include <compare>
#include <initializer_list>
#include <memory>

#include "cont.hpp"
#include "iterator.hpp"

namespace my {

template <typename T>
struct Vector : public Iterator<Vector<T>> {
 private:
  template <typename U>
  struct BaseDynRng {
    U *itr{nullptr};
    U *sen{nullptr};
    U *cap{nullptr};
  };

  BaseDynRng<T> rng{};

 public:
  constexpr Vector() = default;

  constexpr Vector(const Vector &rhs) {
    auto sz{rhs.size()};
    rng.itr = new T[sz]{};
    for (auto b{rhs.begin()}; b != rhs.end(); ++rng.itr, ++b) *rng.itr = *b;
    rng.dyn_rng.sen = rng.itr + sz;
    rng.dyn_rng.cap = rng.itr + sz;
  }

  constexpr auto operator=(const Vector &rhs) -> Vector & {
    if (this != &rhs) {
      delete[] rng.itr;
      auto sz{rhs.size()};
      rng.itr = new T[sz]{};
      for (auto b{rhs.begin()}; b != rhs.end(); ++rng.itr, ++b) *rng.itr = *b;
      rng.sen = rng.itr + sz;
      rng.cap = rng.itr + sz;
    }
    return *this;
  }

  constexpr Vector(Vector &&rhs) noexcept {
    auto sz{rhs.size()};
    rng.itr = new T[sz]{};
    for (auto b{rhs.begin()}; b != rhs.end(); ++rng.itr, ++b)
      *rng.itr = std::exchange(*b, nullptr);
    rng.sen = rng.itr + sz;
    rng.cap = rng.itr + sz;
  }

  constexpr auto operator=(Vector &&rhs) noexcept -> Vector & {
    if (this != &rhs) {
      delete[] rng.itr;
      auto sz{rhs.size()};
      rng.itr = new T[sz]{};
      for (auto b{rhs.begin()}; b != rhs.end(); ++rng.itr, ++b)
        *rng.itr = std::exchange(*b, nullptr);
      rng.sen = rng.itr + sz;
      rng.cap = rng.itr + sz;
    }
    return *this;
  }

  constexpr Vector(const usize sz) : rng{new T[sz]{}} {
    rng.sen = rng.itr + sz;
    rng.cap = rng.itr + sz;
  }

  constexpr Vector(const std::initializer_list<T> args) {
    auto sz{args.size()};
    rng.itr = new T[sz]{};
        std::copy(std::cbegin(args), std::cend(args), rng.itr);
//    for (auto ab{args.begin()}; rng.itr != (rng.itr + sz); ++rng.itr, ++ab)
//      *rng.itr = *ab;
    rng.sen = rng.itr + sz;
    rng.cap = rng.itr + sz;
  }

  template <typename I>
  constexpr Vector(const I iter, const usize len) : rng{new char[len]{}} {
    for (usize i{}; i < len; ++i) rng.itr[i] = iter[i];
    rng.sen = rng.itr + len;
    rng.cap = rng.itr + len;
  }

  /*
    template<typename I>
    constexpr Vector(I i, usize sz) {
      rng.itr = new T[sz]{};
      for (; rng.itr != (rng.itr + sz); ++rng.itr, ++i)
        *rng.itr = *i;
      rng.sen = rng.itr + sz;
      rng.cap = rng.itr + sz;
    }
  */

  template <typename I>
  constexpr Vector(I first, I last) {
    auto dist = first - last;
    static_assert(dist >= 0, "Invalid range");
    rng.itr = new T[dist]{};
    for (; first != last; ++rng.itr, ++first) *rng.itr = *first;
    rng.sen = last;
    rng.cap = last;
  }

  template <usize sz>
  constexpr Vector(T (&a)[sz]) {
    rng.itr = new T[sz]{};
    for (; rng.itr != (rng.itr + sz); ++rng.itr, ++a) *rng.itr = *a;
    rng.sen = rng.itr + sz;
    rng.cap = rng.itr + sz;
  }

  template <typename U>
  constexpr Vector(const Vector<U> &rhs) {
    auto sz{rhs.size()};
    rng.itr = new U[sz]{};
    for (auto b{rhs.begin()}; b != rhs.end(); ++rng.itr, ++b) *rng.itr = *b;
    rng.sen = rng.itr + sz;
    rng.cap = rng.itr + sz;
  }

  constexpr auto operator<=>(const Vector &) const -> bool = default;

  constexpr ~Vector() noexcept { delete[] rng.itr; }

  constexpr auto size() const -> usize { return rng.sen - rng.itr; }

  constexpr auto capacity() const -> usize { return rng.cap - rng.itr; }

  constexpr auto begin() -> T * { return rng.itr; }

  constexpr auto end() -> T * { return rng.sen; }

  constexpr auto operator[](const usize idx) -> T & {
    static_assert(idx < size(), "Size mismatch");
    return rng.itr[idx];
  }

  constexpr auto begin() const -> T * { return rng.itr; }

  constexpr auto end() const -> T * { return rng.sen; }

  constexpr auto operator[](const usize idx) const -> T & {
    static_assert(idx < size(), "Size mismatch");
    return rng.itr[idx];
  }

  constexpr auto push_back(const T &val) -> void {
    if (rng.sen == rng.cap) {
      usize osz = rng.cap - rng.itr;
      usize nsz = osz ? osz * 2 : 2;
      T *tmp{new T[nsz]{}};
      if (rng.itr) {
        //        std::copy(rng.itr, rng.sen, tmp);
        for (; rng.itr != rng.sen; ++rng.itr, ++tmp) *tmp = *rng.itr;
        delete[] rng.itr;
      }
      rng.itr = tmp;
      rng.sen = rng.itr + osz;
      rng.cap = rng.itr + nsz;
    }
    *rng.sen = val;
    rng.sen = rng.sen + 1;
  }

  constexpr auto reserve(usize nsz) -> void {
    auto osz = size();
    T *tmp{new T[nsz]{}};
    if (rng.itr) {
      //      std::copy(rng.itr, rng.sen, tmp);
      for (; rng.itr != (rng.itr + nsz); ++rng.itr, ++tmp) *tmp = *rng.itr;
      delete[] rng.itr;
    }
    rng.itr = tmp;
    rng.sen = tmp + osz;
    rng.cap = tmp + nsz;
  }
};

template <typename T>
Vector(T (&)[]) -> Vector<T>;

template <typename T>
Vector(std::initializer_list<T>) -> Vector<T>;

template <template <typename> typename I, typename T>
Vector(I<T> first, I<T> last) -> Vector<I<T>>;

}  // namespace my

my::Vector<char> operator""_md(const char *ch, const usize len) {
  return my::Vector<char>{ch, len};
}

/*

template<typename T, usize N>
PtrArray(Array<T, N> &) -> PtrArray<T, N>;

template<typename T, usize N>
PtrArray(const Array<T, N> &) -> PtrArray<const T, N>;
*/
/*
 *
//    constexpr Vector(usize _len, const T &_val)  : itr(new T[_len +
20]{}),
//                                                          N(_len),
//                                                          rng.cap(_len + 20) {
//      for (usize i{}; i < _len; ++i) {
//        *(itr + i) = _val;
//      }
//    }
//        template<typename F>
//        constexpr void for_each(const F &f)   {
//            usize i{};
//            for (auto n = begin(), k = end(); n != k; ++i, ++n)
//                f(i, n);
//        }

 //    if (rng.sen == rng.cap) {
//      rng.cap *= 2;
//      T *tmp{new T[rng.cap]{}};
//      for (usize i{}; i < rng.cap; ++i)
//        tmp[i] = itr[i];
//      delete[] itr;
//      itr = tmp;
//    }
//    itr[rng.sen] = val;
//    rng.sen += 1;

*/

/*
template<>
struct Vector<char> {
private:
  BaseDynRng<char> rng{};

public:
  constexpr Vector() = default;

  constexpr Vector(const char *__restrict ch, const usize in_len) :
      rng{new char[in_len]{}} {
    for (usize i{}; i < in_len; ++i)
      rng.itr[i] = ch[i];
    rng.sen = rng.itr + in_len;
    rng.cap = rng.itr + in_len;
  }

  constexpr ~Vector() noexcept {
    delete[] rng.itr;
  }

  auto size() -> usize {
    return rng.sen - rng.itr;
  }
  auto begin() -> char * {
    return rng.itr;
  }
  template<typename OS>
  friend auto operator<<(OS &os, const Vector &rhs) -> OS & {
    os << '\"';
    for (auto x{rhs.rng.itr}; x != rhs.rng.sen; ++x)
      os << *x;
    return os << '\"';
  }
};
*/
