#pragma once

#include <QDebug>
#include <vector>
#include <initializer_list>

#include "vector.h"

namespace bm {

class World;
class ComponentObject;

namespace ai {

class Tribool {
public:
    typedef enum { False = 0, True = 1, N_A = 2 } Value;

    explicit Tribool(): val_(False) {}
    explicit Tribool(bool x): val_(x ? True : False) {}
    Tribool(Value v): val_(v) {}

    bool is_false() const { return val_ == False; }
    bool is_true() const { return val_ == True; }
    bool is_NA() const { return val_ == N_A; }

private:
    Value val_;
};

//enum class State: uint8_t {
//    Moving,
//    Using,
//};

// Desired effects
enum class MetricType: uint16_t {
    NearPosition,   // Creature moved to be in reach
    BlockIsNotSolid, // Block was extracted using tools or blasted in some way
    HaveLeg,        // at least one leg
    HaveHand,       // at least one hand
    HaveMiningPick, // a tool
};

//enum class Check: uint8_t {
//    IsFalse,
//    IsTrue,
//    Equal,
//    //Greater,
//};

// A 3d vector with trivial ctor (unlike Vec3i)
class Pos3i {
public:
    int32_t x, y, z;
    Pos3i(const Vec3i &v): x(v.getX()), y(v.getY()), z(v.getZ()) {}
    bool operator== (const Pos3i& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

class Value {
private:
    enum class Type: uint8_t {
        NoValue,
        Boolean,
        Position
    };
    union {
        bool  b_;
        Pos3i pos_;
    };
    Type type_;
public:
    Value(const Value &) = default;
    explicit Value(): type_(Type::NoValue) {}
    Value(const Vec3i &p): pos_(p), type_(Type::Position) {}
    Value(bool b): b_(b), type_(Type::Boolean) {}

    bool is_position() const { return type_ == Type::Position; }
    Vec3i get_pos() const;

    bool operator== (const Value& other) const {
        if (type_ != other.type_) {
            return false;
        }
        switch (type_) {
        case Type::NoValue:     return true;
        case Type::Position:    return pos_ == other.pos_;
        case Type::Boolean:     return b_ == other.b_;
        }
    }
};

// A value which is desired by some plan, or which is currently present.
class Metric {
public:
    MetricType  type_;
    Value       arg_;

    explicit Metric(MetricType ct): type_(ct), arg_() {}
    explicit Metric(MetricType ct, Value arg): type_(ct), arg_(arg) {}

    bool operator== (const Metric& other) const {
        Q_ASSERT(type_ == other.type_);
        return arg_ == other.arg_;
    }
};

// TODO: a fixed-position array or a bitmap+metricvec combination maybe?
using MetricVec = std::vector<Metric>;

}} // ns ai::bm
