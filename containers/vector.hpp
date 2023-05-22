#pragma once
#include <compare>
#include <initializer_list>

#include "../utilities/iterator.hpp"

namespace my {

template<typename T>
struct Vector : public Iterator<Vector<T>> {
private:
    T *itr{nullptr};
    T *sen{nullptr};
    T *cap{nullptr};

public:
    constexpr Vector() = default;

    constexpr Vector(const Vector &rhs) {
        auto sz = rhs.size();
        itr = new T[sz];
        copy(rhs.itr, rhs.sen, itr);
        sen = itr + sz;
        cap = rhs + sz;
    }

    constexpr auto operator=(const Vector &rhs) -> Vector & {
        if (this != &rhs) {
            delete[] itr;
            auto sz = rhs.size();
            itr = new T[sz];
            copy(rhs.itr, rhs.sen, itr);
            sen = itr + sz;
            cap = rhs + sz;
        }
        return *this;
    }

    constexpr Vector(Vector &&rhs) noexcept : itr{exchange(rhs.itr, nullptr)}, sen{move(rhs.sen)}, cap{move(rhs.cap)} {}

    constexpr auto operator=(Vector &&rhs) -> Vector & {
        if (this != &rhs) {
            delete[] itr;
            itr = exchange(rhs.itr, nullptr);
            sen = move(rhs.sen);
            cap = move(rhs.cap);
        }
        return *this;
    }

    constexpr Vector(const usize sz) : itr{new T[sz]{}} {
        sen = itr + sz;
        cap = itr + sz;
    }

    constexpr Vector(const std::initializer_list<T> args) {
        auto sz{args.size()};
        itr = new T[sz]{};
        copy(args.begin(), args.end(), itr);
        sen = itr + sz;
        cap = itr + sz;
    }

    template<typename I>
    constexpr Vector(const I *i, const usize len) : itr{new I[len]} {
        copy(i, len, itr);
        sen = itr + len;
        cap = itr + len;
    }

    template<typename I>
    constexpr Vector(const I *first, const I *last) {
        auto dist = first - last;
        itr = new I[dist];
        copy(first, last, itr);
        sen = itr + dist;
        cap = itr + dist;
    }

    template<usize sz>
    constexpr Vector(T (&a)[sz]) {
        itr = new T[sz]{};
        copy(a, sz, itr);
        sen = itr + sz;
        cap = itr + sz;
    }

    constexpr auto push_back(const T &val) -> void {
        if (sen == cap) {
            usize osz = cap - itr;
            usize nsz = osz ? osz * 2 : 2;
            T *tmp{new T[nsz]{}};
            if (itr) {
                copy(itr, sen, tmp);
                delete[] itr;
            }
            itr = tmp;
            sen = itr + osz;
            cap = itr + nsz;
        }
        *sen = val;
        sen += 1;
    }

    constexpr auto reserve(usize nsz) -> void {
        auto osz = size();
        T *tmp{new T[nsz]{}};
        if (itr) {
            copy(itr, sen, tmp);
            delete[] itr;
        }
        itr = tmp;
        sen = tmp + osz;
        cap = tmp + nsz;
    }

    constexpr auto operator<=>(const Vector &) const -> bool = default;

    constexpr ~Vector() noexcept { delete[] itr; }

    constexpr auto front() -> T & { return itr; }

    constexpr auto back() -> T & { return sen; }

    constexpr auto front() const -> T & { return itr; }

    constexpr auto back() const -> T & { return sen; }

    constexpr auto size() const -> usize { return sen - itr; }

    constexpr auto capacity() const -> usize { return cap - itr; }

    constexpr auto begin() -> T * { return itr; }

    constexpr auto end() -> T * { return sen; }

    constexpr auto operator[](const usize idx) -> T & { return itr[idx]; }

    constexpr auto begin() const -> T * { return itr; }

    constexpr auto end() const -> T * { return sen; }

    constexpr auto operator[](const usize idx) const -> T & { return itr[idx]; }
};

template<typename T>
Vector(T (&)[]) -> Vector<T>;

template<typename T>
Vector(std::initializer_list<T>) -> Vector<T>;

template<template<typename> typename I, typename T>
Vector(I<T> first, I<T> last) -> Vector<I<T>>;

}  // namespace my

my::Vector<char> operator""_md(const char *ch, const my::usize len) {
    return my::Vector<char>{ch, len};
}
