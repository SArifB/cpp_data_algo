//
// Created by sayea on 5/20/2023.
//

#pragma once

namespace my {

template<class T>
struct Ref {
private:
    T *ptr;

public:
//    constexpr Ref() = delete;
//    constexpr Ref(nullptr_t) = delete;
//    constexpr Ref(const T &&val) = delete;
//
//    constexpr Ref(const Ref &rhs) noexcept = default;
//
//    constexpr auto operator=(const Ref &rhs) noexcept -> Ref & = default;
//
//    constexpr Ref(Ref &&rhs) noexcept = default;
//
//    constexpr auto operator=(Ref &&rhs) noexcept -> Ref & = default;

    constexpr Ref(T &&val) noexcept {
        T &ref = move(val);
        ptr = &ref;
    }

    constexpr ~Ref() noexcept = default;

    constexpr operator T &() const noexcept { return *ptr; }

    operator bool() const { return static_cast<bool>(ptr); }

    constexpr auto get() const noexcept -> T & { return *ptr; }

    template<class... Args>
    constexpr auto operator()(Args &&...args) const noexcept
        -> decltype(ptr(static_cast<Args &&>(args)...)) {
        return ptr(static_cast<Args &&>(args)...);
    }
};

template<class T>
Ref(T) -> Ref<T>;

}  // namespace my
