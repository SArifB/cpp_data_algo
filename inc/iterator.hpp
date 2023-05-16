#pragma once
#include "cont.hpp"

namespace my {

template <typename D>
struct Iterator {
 private:
  inline constexpr auto self() -> D && { return static_cast<D &&>(*this); }

  inline constexpr auto self() const -> const D && {
    return static_cast<const D &&>(*this);
  }

 protected:
  constexpr ~Iterator() = default;

 public:
  template <typename F>
  constexpr auto for_each(F &&f) -> void {
    for (auto i{self().begin()}, k{self().end()}; i != k; ++i) f(i);
  }

  template <typename F>
  constexpr auto for_each(F &&f) const -> void {
    for (auto i{self().begin()}, k{self().end()}; i != k; ++i) f(i);
  }

  template <usize bn, usize nd, typename F>
  constexpr auto for_each(F &&f) -> void {
    for (auto i{self().begin() + bn}, k{self().end() + nd}; i != k; ++i) f(i);
  }

  template <typename OS>
  friend auto operator<<(OS &os, const D &rhs) -> OS & {
    os << '[' << ' ';
    for (auto x{rhs.begin()}; x != rhs.end(); ++x) os << *x << ", ";
    return os << ']';
  }

  //  template<typename OS>
  //  friend auto operator<<(OS &os, const D &rhs) -> OS & {
  //    os << '\"';
  //    for (auto x{rhs.begin()}; x != rhs.rng.sen; ++x) os << *x;
  //    return os << '\"';
  //  }
};
}  // namespace my

/*
         for (usize i{}; i != N; ++i) os << rhs.itr[i] << ", ";
   //    inline constexpr auto self() const   -> const D && { return
   static_cast<const D &&>(*this); }

   //    template<typename T>
   //    constexpr auto front()   -> T {
   //      return (static_cast<D *>(this))->begin();
   //    }
   //
   //    template<typename T>
   //    constexpr auto back()   -> T {
   //      return (static_cast<D *>(this))->end();
   //    }
   inline constexpr auto self() const   -> const D * { return static_cast<D
   *>(this); } //static_cast<D &>(*this). for (auto i{(static_cast<D
   *>(this))->begin()}, k{(static_cast<D *>(this))->end()}; i != k; struct Base
   { void name(this auto&& self) { self.impl(); } }; struct D1 : public Base {
   void impl() { std::puts("D1::impl()"); } }; struct D2 : public Base { void
   impl() { std::puts("D2::impl()"); } };

   constexpr void name(this auto &&self) { self.next(); }

   template <typename T>
   struct optional {
     // One version of value which works for everything
     template <class Self>
     constexpr auto&& value(this Self&& self) {
       if (self.has_value()) {
           return std::forward<Self>(self).m_value;
       }
       throw bad_optional_access();
     }
   };
   */
