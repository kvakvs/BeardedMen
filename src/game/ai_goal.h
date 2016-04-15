#pragma once

#include <vector>
#include "vector.h"

namespace bm {namespace ai {

enum class State: uint8_t {
    Moving,
    Using,
};

// Desired effects
enum class EffectType: uint16_t {
    NearPosition,   // Creature moved to be in reach
    BlockMined,     // A rock block was extracted using tools
};

enum class Check: uint8_t {
    False,
    True,
    Equal,
    //Greater,
};

class Value {
    union {
        Vec3i   pos_;
    };
};

// Condition represents some check. Value of eff_ must be pass the check_(arg_)
class Condition {
public:
    EffectType  eff_;
    Check       check_;
    //Value       arg_;
};

class Step {
public:
    // Precondition, what must be present
    Condition precond_;
    // Desired effect: what we must do
    EffectType desired_;
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
