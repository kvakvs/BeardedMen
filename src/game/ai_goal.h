#pragma once

#include <vector>
#include <initializer_list>

#include "vector.h"

namespace bm {
class World;

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

enum class State: uint8_t {
    Moving,
    Using,
};

// Desired effects
enum class CondType: uint16_t {
    AlwaysTrue,
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
        NoValue,
        Position
    };
    union {
        Pos3i pos_;
    };
    Type type_;
public:
    Value(): type_(Type::NoValue) {}
    Value(const Value &) = default;
    Value(const Vec3i &p): pos_(p), type_(Type::Position) {}

    bool is_position() const { return type_ == Type::Position; }
    Vec3i get_pos() const;
};

// Condition represents some check. Value of eff_ must pass the check_(arg_)
// Conditions can also represent orders and desires of player and creatures
class Condition {
public:
    CondType cond_;
    Check    check_;
    Value    arg_;

    static Condition make_always_true() {
        return Condition(CondType::AlwaysTrue);
    }

    explicit Condition(CondType ef): cond_(ef), arg_() {}
    Condition(CondType ef, Check chk, Value arg)
        : cond_(ef), check_(chk), arg_(arg) {}

    // Using world's state (read-only) check if condition stands true
    // This is global world check without entity reference, and it will fail
    // for all entity conditions like their position or state.
    Tribool is_fulfilled_glob(const World &wo) const;
};

// Goal is what we want done (in desired_). Preconditions are to be checked
// before and after attempting the action.
class Goal {
public:
    // Precondition, what must be present
    std::vector<Condition> precond_;
    // Desired effect: what we must do
    Condition desired_;

    static Goal make_empty() { return Goal(); }
    explicit Goal(): desired_(Condition::make_always_true()) {}
    explicit Goal(std::initializer_list<Condition> conds, Condition want)
        : precond_(conds), desired_(want) {}

    bool has_preconditions_glob(const World& wo) const;
    bool is_fulfilled_glob(const World& wo) const {
        return not desired_.is_fulfilled_glob(wo).is_false();
    }
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
