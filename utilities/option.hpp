//
// Created by sayea on 5/20/2023.
//

#pragma once
#include "../containers/inc.hpp"

namespace {
struct Empty {};
}  // namespace ~

namespace my {

template<typename T>
struct Opt {
private:
    union {
        T val;
        Empty e;
    };
public:
    constexpr Opt(): e{} {}

    constexpr Opt(const auto &...args) : val{args...} {}

    constexpr Opt(auto &&...args) : val{move(args)...} {}

    constexpr Opt(const Opt &) = default;

    constexpr auto operator=(const Opt &) -> Opt & = default;

    constexpr Opt(Opt &&) = default;

    constexpr auto operator=(Opt &&) -> Opt & = default;

    constexpr auto operator<=>(const Opt &) const -> bool = default;

    constexpr ~Opt() = default;

    constexpr operator T &() const noexcept { return val; }

    operator bool() const { return static_cast<bool>(val); }

    constexpr auto get() const noexcept -> T & { return val; }


};
//
//template<>
//struct Opt<Empty> {
//private:
////    [[maybe_unused]] [[no_unique_address]] Empty val;
//public:
//};


template<typename T>
Opt(T) -> Opt<T>;

}  // namespace my
