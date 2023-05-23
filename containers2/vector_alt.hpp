//
// Created by sayea on 5/23/2023.
//

#pragma once
#include "../pointers/box_ptr.hpp"
#include "../utilities/utility.hpp"
#include "../utilities/iterator.hpp"
#include <initializer_list>

namespace my_alt {

template<typename T> struct Vector : public my::Iterator<Vector<T>>{
private:
    using Box = my::Box<T[]>;
    using usize = my::usize;

    Box ptr{};
    usize len{};
    usize cap{};

public:
    constexpr Vector() = default;

    constexpr Vector(const Vector &rhs) : ptr{Box{rhs.len}}, len{rhs.len}, cap{rhs.cap} {
        my::copy(rhs.ptr.get(), rhs.len, ptr.get());
    }

    constexpr Vector(Vector &&rhs) noexcept : ptr{my::move(rhs.ptr)}, len{my::move(rhs.len)}, cap{my::move(rhs.cap)} {}

    constexpr auto operator=(Vector rhs) noexcept -> Vector & {
        rhs.swap(*this);
        return *this;
    }

    constexpr Vector(const usize sz) : ptr{Box{sz}}, len{sz}, cap{sz} {}

    constexpr Vector(const std::initializer_list<T> args) : ptr{Box{args.size()}}, len{args.size()}, cap{args.size()} {
        my::copy(args.begin(), args.end(), ptr.get());
    }

    constexpr Vector(const T *_ptr, const usize _len) : ptr{Box{_len}}, len{_len}, cap{_len} {
        copy(_ptr, _len, ptr.get());
    }

    constexpr Vector(const T *first, const T *last) : ptr{Box{last - first}}, len{last - first}, cap{last - first} {
        my::copy(first, last, ptr.get());
    }

    template<usize sz>
    constexpr explicit Vector(T (&a)[sz]) : ptr{Box{sz}}, len{sz}, cap{sz} {
        my::copy(a, sz, ptr);
    }

    constexpr auto push_back(T &&val) -> void {
        if (len == cap) {
            usize nsz = len ? len * 2 : 2;
            auto tmp = Box{nsz};
            if (ptr) my::copy(ptr.get(), len, tmp.get());
            ptr = my::move(tmp);
            cap = nsz;
        }
        ptr[len++] = my::move(val);
    }

    constexpr auto reserve(const usize nsz) -> void {
        auto tmp = Box{nsz};
        if (ptr) my::copy(ptr.get(), len, tmp.get());
        ptr = my::move(tmp);
        cap = nsz;
    }

    constexpr auto operator<=>(const Vector &) const -> bool = default;

    constexpr ~Vector() noexcept = default;

    constexpr auto swap(Vector &rhs) -> void {
        my::swap(ptr, rhs.ptr);
        my::swap(len, rhs.len);
        my::swap(cap, rhs.cap);
    }

    constexpr auto swap(Vector &left, Vector &right) -> void { left.swap(right); }

    constexpr auto front() -> T & { return *ptr; }

    constexpr auto back() -> T & { return ptr[len - 1]; }

    constexpr auto front() const -> T & { return *ptr; }

    constexpr auto back() const -> T & { return ptr[len - 1]; }

    constexpr auto size() const -> usize { return len; }

    constexpr auto capacity() const -> usize { return cap; }

    constexpr auto operator[](const usize idx) -> T & { return ptr[idx]; }

    constexpr auto begin() const -> T * { return ptr.get(); }

    constexpr auto end() const -> T * { return ptr.get() + len; }

    constexpr auto operator[](const usize idx) const -> T & { return ptr[idx]; }
};
}  // namespace my_alt
