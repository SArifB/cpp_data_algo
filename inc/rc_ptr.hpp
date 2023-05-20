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
struct WkRc;

template<typename T>
struct Rc {
private:
    CtrlBlock<T> *cb{nullptr};

public:
    Rc() : cb{new CtrlBlock<T>{1}} {}

    Rc(T &&_val) : cb{new CtrlBlock<T>{1, 0, std::move(_val)}} {}

    Rc(T *_val) : cb{new CtrlBlock<T>{1, 0, *_val}} {}

    Rc(const Rc &rhs) : cb{rhs.cb} { cb->srefs += 1; }

    Rc(const WkRc<T> &rhs) : cb{rhs.data()} { cb->srefs += 1; }

    auto operator=(const Rc &rhs) -> Rc & {
        if (this != &rhs) {
            cb = rhs.cb;
            cb->srefs += 1;
        }
        return *this;
    }

    auto operator=(const WkRc<T> &rhs) -> Rc & {
        if (this != &rhs) {
            cb = rhs.data();
            cb->srefs += 1;
        }
        return *this;
    }

    Rc(Rc &&rhs) : cb{std::move(rhs.cb)} { cb->srefs += 1; }

    Rc(WkRc<T> &&rhs) : cb{std::move(rhs.data())} { cb->srefs += 1; }

    auto operator=(Rc &&rhs) -> Rc & {
        if (this != &rhs) {
            cb = std::move(rhs.cb);
            cb->srefs += 1;
        }
        return *this;
    }

    auto operator=(WkRc<T> &&rhs) -> Rc & {
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
struct WkRc {
private:
    CtrlBlock<T> *cb;

public:
    WkRc() = delete;

    WkRc(const WkRc &rhs) : cb{rhs.cb} { cb->wrefs += 1; }

    WkRc(const Rc<T> &rhs) : cb{rhs.data()} { cb->wrefs += 1; }

    auto operator=(const WkRc &rhs) -> WkRc & {
        if (this != &rhs) {
            cb = rhs.cb;
            cb->wrefs += 1;
        }
        return *this;
    }

    auto operator=(const Rc<T> &rhs) -> WkRc & {
        if (this != &rhs) {
            cb = rhs.data();
            cb->wrefs += 1;
        }
        return *this;
    }

    WkRc(WkRc &&rhs) : cb{std::move(rhs.cb)} { cb->wrefs += 1; }

    WkRc(Rc<T> &&rhs) : cb{std::move(rhs.data())} { cb->wrefs += 1; }

    auto operator=(WkRc &&rhs) -> WkRc & {
        if (this != &rhs) {
            cb = std::move(rhs.cb);
            cb->wrefs += 1;
        }
        return *this;
    }

    auto operator=(Rc<T> &&rhs) -> WkRc & {
        if (this != &rhs) {
            cb = std::move(rhs.data());
            cb->wrefs += 1;
        }
        return *this;
    }

    ~WkRc() { cb->wrefs -= 1; }

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
WkRc(T *) -> WkRc<T>;

}  // namespace my