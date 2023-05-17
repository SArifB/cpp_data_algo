#pragma once
#include <utility>

#include "cont.hpp"

namespace my {

template<typename T>
struct CtrlBlock {
    usize srefs{};
    usize wrefs{};
    T val{};
};

template<typename T>
struct wRc;

template<typename T>
struct Rc {
private:
    CtrlBlock<T> *cb{nullptr};

public:
    Rc() : cb{new CtrlBlock<T>{1}} {}

    Rc(T &&_val) : cb{new CtrlBlock<T>{1, 0, std::move(_val)}} {}

    Rc(T *_val) : cb{new CtrlBlock<T>{1, 0, *_val}} {}

    Rc(const Rc &rhs) : cb{rhs.cb} { cb->srefs += 1; }

    Rc(const wRc<T> &rhs) : cb{rhs.data()} { cb->srefs += 1; }

    auto operator=(const Rc &rhs) -> Rc & {
        if (this != &rhs) {
            cb = rhs.cb;
            cb->srefs += 1;
        }
        return *this;
    }

    auto operator=(const wRc<T> &rhs) -> Rc & {
        if (this != &rhs) {
            cb = rhs.data();
            cb->srefs += 1;
        }
        return *this;
    }

    Rc(Rc &&rhs) : cb{std::move(rhs.cb)} { cb->srefs += 1; }

    Rc(wRc<T> &&rhs) : cb{std::move(rhs.data())} { cb->srefs += 1; }

    auto operator=(Rc &&rhs) -> Rc & {
        if (this != &rhs) {
            cb = std::move(rhs.cb);
            cb->srefs += 1;
        }
        return *this;
    }

    auto operator=(wRc<T> &&rhs) -> Rc & {
        if (this != &rhs) {
            cb = std::move(rhs.data());
            cb->srefs += 1;
        }
        return *this;
    }

    ~Rc() {
        cb->srefs -= 1;
        if (cb->srefs < 1) 
            delete cb;
    }

    auto data() -> CtrlBlock<T> * { return cb; }

    auto data() const -> CtrlBlock<T> * { return cb; }

    operator bool() const { return static_cast<bool>(cb); }

    auto strong_count() -> usize { return cb->srefs; }

    auto weak_count() -> usize { return cb->wrefs; }

    auto get() const -> T * { return &(cb->val); }

    auto operator->() const -> T * { return &(cb->val); }

    auto operator*() -> T & { return cb->val; }
};

template<typename T>
struct wRc {
private:
    CtrlBlock<T> *cb;

public:
    wRc() = delete;

    wRc(const wRc &rhs) : cb{rhs.cb} { cb->wrefs += 1; }

    wRc(const Rc<T> &rhs) : cb{rhs.data()} { cb->wrefs += 1; }

    auto operator=(const wRc &rhs) -> wRc & {
        if (this != &rhs) {
            cb = rhs.cb;
            cb->wrefs += 1;
        }
        return *this;
    }

    auto operator=(const Rc<T> &rhs) -> wRc & {
        if (this != &rhs) {
            cb = rhs.data();
            cb->wrefs += 1;
        }
        return *this;
    }

    wRc(wRc &&rhs) : cb{std::move(rhs.cb)} { cb->wrefs += 1; }

    wRc(Rc<T> &&rhs) : cb{std::move(rhs.data())} { cb->wrefs += 1; }

    auto operator=(wRc &&rhs) -> wRc & {
        if (this != &rhs) {
            cb = std::move(rhs.cb);
            cb->wrefs += 1;
        }
        return *this;
    }

    auto operator=(Rc<T> &&rhs) -> wRc & {
        if (this != &rhs) {
            cb = std::move(rhs.data());
            cb->wrefs += 1;
        }
        return *this;
    }

    ~wRc() { cb->wrefs -= 1; }

    auto data() -> CtrlBlock<T> * { return cb; }

    auto data() const -> CtrlBlock<T> * { return cb; }

    operator bool() const { return static_cast<bool>(cb); }

    auto strong_count() -> usize { return cb->srefs; }

    auto weak_count() -> usize { return cb->wrefs; }

    auto get() const -> T * { return &(cb->val); }

    auto operator->() const -> T * { return &(cb->val); }

    auto operator*() -> T & { return cb->val; }
};

template<typename T>
Rc(T *) -> Rc<T>;

template<typename T>
wRc(T *) -> wRc<T>;

}  // namespace my