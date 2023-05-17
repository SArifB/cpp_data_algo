#pragma once
#include "cont.hpp"

namespace my {

template<typename D>
struct Iterator {
  private:
    inline constexpr auto self() -> D && { return static_cast<D &&>(*this); }

    inline constexpr auto self() const -> const D && {
        return static_cast<const D &&>(*this);
    }

  protected:
    constexpr ~Iterator() = default;

  public:
    template<typename F>
    constexpr auto for_each(F &&f) -> void {
        for (auto i{self().begin()}, k{self().end()}; i != k; ++i) f(i);
    }

    template<typename F>
    constexpr auto for_each(F &&f) const -> void {
        for (auto i{self().begin()}, k{self().end()}; i != k; ++i) f(i);
    }

    template<usize bn, usize nd, typename F>
    constexpr auto for_each(F &&f) -> void {
        for (auto i{self().begin() + bn}, k{self().end() + nd}; i != k; ++i) f(i);
    }

    template<typename OS>
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
