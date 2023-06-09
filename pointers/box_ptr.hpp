#pragma once

#include "../utilities/utility.hpp"

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
class Box<T[]> {
private:
    T *ptr{nullptr};

public:
    Box() = default;

    Box(T *_ptr) : ptr(_ptr) {}

    Box(usize sz) : ptr(new T[sz]{}) {}

    Box(const Box &) = delete;

    auto operator=(const Box &) -> Box & = delete;

    Box(Box &&rhs) noexcept : ptr(rhs.release()) {}

    auto operator=(Box &&rhs) noexcept -> Box & {
        if (this != &rhs) reset(rhs.release());
        return *this;
    }

    ~Box() noexcept { delete[] ptr; }

    auto release() -> T * { return my::exchange(ptr, nullptr); }

    auto reset(T *_ptr = nullptr) -> void {
        T *tmp = my::exchange(ptr, _ptr);
        delete[] tmp;
    }

    operator bool() const { return static_cast<bool>(ptr); }

    auto operator++() -> Box& {
        ++ptr;
        return *this;
    }

    auto operator++(int) -> Box& {
        auto tmp = *this;
        ++ptr;
        return tmp;
    }

    auto get() const -> T * { return ptr; }

    auto operator->() const -> T * { return ptr; }

    auto operator[](const usize idx) const -> T & { return ptr[idx]; }

    auto operator*() -> T & { return *ptr; }
};

template<typename T>
Box(T *) -> Box<T>;

}  // namespace my