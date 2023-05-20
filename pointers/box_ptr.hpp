#pragma once

#include "../containers/inc.hpp"

namespace my {

template<typename T>
class Box {
private:
    T *ptr{nullptr};

public:
    Box() = default;

    Box(T *_ptr) : ptr(_ptr) {}

    Box(const Box &) = delete;

    auto operator=(const Box &) -> Box & = delete;

    Box(Box &&rhs) noexcept : ptr(rhs.release()) {}

    auto operator=(Box &&rhs) noexcept -> Box & {
        if (this != &rhs) reset(rhs.release());
        return *this;
    }

    ~Box() noexcept { delete ptr; }

    auto release() -> T * { return exchange(ptr, nullptr); }

    auto reset(T *_ptr = nullptr) -> void {
        T *tmp = exchange(ptr, _ptr);
        delete tmp;
    }

    operator bool() const { return static_cast<bool>(ptr); }

    auto get() const -> T * { return ptr; }

    auto operator->() const -> T * { return ptr; }

    auto operator*() -> T & { return *ptr; }
};

template<typename T>
Box(T *) -> Box<T>;


}  // namespace my