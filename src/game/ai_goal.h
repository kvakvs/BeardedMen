#pragma once

#include <vector>
#include "vector.h"

namespace bm {
class World;

namespace ai {

enum class State: uint8_t {
    Moving,
    Using,
};

// Desired effects
enum class CondType: uint16_t {
    NearPosition,   // Creature moved to be in reach
    BlockMined,     // A rock block was extracted using tools
};

enum class Check: uint8_t {
    IsFalse,
    IsTrue,
    Equal,
    //Greater,
};

// A 3d vector with trivial ctor (unlike Vec3i)
class Pos3i {
public:
    int32_t x, y, z;
    Pos3i(const Vec3i &v): x(v.getX()), y(v.getY()), z(v.getZ()) {}
};

class Value {
private:
    enum class Type: uint8_t {
        Position
    };
    union {
        Pos3i pos_;
    };
    Type type_;
public:
    Value(const Value &) = default;
    Value(const Vec3i &p): pos_(p), type_(Type::Position) {}

    bool is_position() const { return type_ == Type::Position; }
    Vec3i get_pos() const;
};

// Condition represents some check. Value of eff_ must be pass the check_(arg_)
class Condition {
public:
    CondType eff_;
    Check    check_;
    Value    arg_;

    Condition(CondType ef, Check chk, Value arg)
        : eff_(ef), check_(chk), arg_(arg) {
    }
    // Using world's state (read-only) check if condition stands true
    // This is global world check without entity reference, and it will fail
    // for all entity conditions like their position or state.
    bool is_fulfilled_glob(const World &wo) const;
};

class Step {
public:
    // Precondition, what must be present
    std::vector<Condition> precond_;
    // Desired effect: what we must do
    CondType desired_;
};

enum class GoalType: uint16_t {
    PerformMining,
};

// Have list of goals
// Select one goal
// Think list of actions from available actions list

//class Plan {
//public:
//    std::vector<GoalType> goals_;
    //std::vector<Step> steps_;
    //virtual bool is_goal_reached() const = 0;
//};

}} // ns ai::bm
