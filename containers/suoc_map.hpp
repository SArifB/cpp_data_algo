//
// Created by sayea on 5/20/2023.
//

#pragma once
#include "inc.hpp"

namespace my {

template<typename T, typename U, usize N = 8>
struct Map {
    using Pair_tp = Pair<T, U>;
    Array<Pair_tp, N> data{};
    usize len{};

    constexpr Map() = default;
    constexpr ~Map() = default;

    constexpr auto operator[](const T &rval) -> U & { return find(begin(), end(), rval)->val; }
    constexpr auto operator[](const T &rval) const -> const U & { return find(begin(), end(), rval)->val; }

    constexpr auto push_back(Pair<T, U> &&val) -> void { data[len++] = move(val); }
    constexpr auto push_back(const Pair<T, U> &val) -> void { data[len++] = val; }

    constexpr auto push_back(T &&key, U &&val) -> void { data[len++] = Pair_tp{move(key), move(val)}; }
    constexpr auto push_back(const T &key, const U &val) -> void { data[len++] = Pair_tp{key, val}; }

    constexpr auto size() -> usize { return len; }
    constexpr auto begin() -> Pair_tp * { return data.begin(); }
    constexpr auto end() -> Pair_tp * { return data.begin() + len; };
};

} // namespace my
