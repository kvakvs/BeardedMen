#pragma once
//#include <QDebug>
#include <vector>
#include <initializer_list>
#include <map>

#include "tab/t_metric.h"
#include "util/error.h"
#include "util/vec.h"
#include "model_id.h"

namespace bm {

class World;
class AnimateObject;

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

// A 3d vector with trivial ctor (unlike Vec3i)
class Pos3i {
public:
    int32_t x, y, z;
    Pos3i(const Vec3i &v): x(v.getX()), y(v.getY()), z(v.getZ()) {}
    bool operator== (const Pos3i& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

/*inline QDebug operator<< (QDebug d, const Pos3i& p) {
    d.nospace() << "Pos(" << p.x << "," << p.y << "," << p.z << ")";
    return d;
}*/

class Value {
public:
    enum class Type: uint8_t {
        NoValue,
        Boolean,
        Position
    };

private:
    union {
        bool  b_;
        Pos3i pos_;
    };
    Type type_;
public:
    Value(const Value &) = default;
    explicit Value(): type_(Type::NoValue) {}
    explicit Value(const Vec3i &p): pos_(p), type_(Type::Position) {}
    explicit Value(bool b): b_(b), type_(Type::Boolean) {}

    bool has_value() const { return type_ != Type::NoValue; }

    bool is_position() const { return type_ == Type::Position; }
    Vec3i get_pos() const;

    Type get_type() const { return type_; }
    bool get_boolean() const {
        BM_ASSERT(type_ == Type::Boolean);
        return b_;
    }

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

//QDebug operator<< (QDebug d, const Value& v);

// A value which is desired by some plan, or which is currently present.
class Metric {
public:
    MetricType  type_;
    Value       arg_;       // optional argument for metric
    Value       reading_;   // metric reading (actual or desired value)

    explicit Metric(MetricType ct): type_(ct) {}
    explicit Metric(MetricType ct, Value arg)
        : type_(ct), arg_(arg) {}
    explicit Metric(MetricType ct, Value arg, Value reading)
        : type_(ct), arg_(arg), reading_(reading) {}

    template <typename SomeValue>
    Metric set_reading(const SomeValue &v) const {
        Metric m(*this);
        m.reading_ = ai::Value(v);
        return m;
    }

    bool operator== (const Metric& other) const {
        BM_ASSERT(type_ == other.type_);
        BM_ASSERT(arg_ == other.arg_);
        return reading_ == other.reading_;
    }
};

//QDebug operator<< (QDebug d, const Metric &m);

// TODO: a fixed-position array or a bitmap+metricvec combination maybe?
using MetricVec = std::vector<Metric>;

//QDebug operator<< (QDebug d, const MetricVec &metrics);

// Context for metric (who is acting)
class Context {
public:
    //const World* world_ = nullptr;
    const AnimateObject* actor_ = nullptr;
    Vec3i pos_;

    Context() {}
    Context(const AnimateObject *act)
        : actor_(act) {}
};

//using Order = std::pair<MetricVec, Context>;

using OrderId = uint64_t;

// Desired metrics paired with execution context
class Order {
    static uint64_t last_id_;
public:
    OrderId     id_;
    MetricVec   desired_;
    Context     ctx_;
    struct {
        ModelId model_id_;
    } visual_;

    using Ptr = std::shared_ptr<Order>;

    Order() = delete;               // nocreate
    Order(const Order&) = delete;   // nocopy
    Order(const MetricVec& m, const Context& c, ModelId vis_m)
        : desired_(m), ctx_(c), visual_ {vis_m} {
        id_ = last_id_++;
    }
};

using OrderMap = std::map<OrderId, Order::Ptr>;

}} // ns ai::bm
