//
// Created by sayea on 5/20/2023.
//

#pragma once

namespace my {

template<typename T, typename U>
struct Pair {
private:
    T key{};
    U val{};

public:
    constexpr Pair() = default;
    constexpr ~Pair() = default;
    constexpr Pair(const Pair &) = default;
    constexpr auto operator=(const Pair &) -> Pair& = default;
    constexpr Pair(Pair &&) noexcept = default;
    constexpr auto operator=(Pair &&) noexcept -> Pair& = default;
    //    constexpr Pair(const Pair &rhs) : key{rhs.key}, val{rhs.val} {}
    //    constexpr Pair( Pair &&rhs) : key{move(rhs.key)}, val{move(rhs.val)} {}

    constexpr Pair(const T &_key, const U &_val) : key{_key}, val{_val} {}
    constexpr Pair(T &&_key, U &&_val) : key{move(_key)}, val{move(_val)} {}
    constexpr bool operator==(const T &rval) const { return key == rval; }
};

template<typename T, typename U>
Pair(const T&, const U&) -> Pair<T, U>;
template<typename T, typename U>
Pair(T&&, U&&) -> Pair<T, U>;

} // namespace my
