#pragma once

namespace bm {

template <typename T>
class Optional {
    T value_;
    bool has_value_ = false;
public:
    Optional() {}
    Optional(const T& v): value_(v), has_value_(true) {}

    bool has_value() const { return has_value_; }
    T &get_value() { return value_; }
    const T &get_value() const { return value_; }
};

} // ns bm
