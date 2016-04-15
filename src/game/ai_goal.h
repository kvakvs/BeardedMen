#pragma once

#include <vector>

namespace bm {namespace ai {

enum class State: uint8_t {
    Moving,
    Using,
};

// Desired effects
enum class EffectType: uint16_t {
    PositionChanged,
    BlockMined,
};

enum class Check: uint8_t {
    False,
    True,
    Equal,
    Greater,
};

class Value {

};

// Condition represents some check. Value of eff_ must be pass the check_(arg_)
class Condition {
public:
    EffectType  eff_;
    Check       check_;
    Value       arg_;
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

class Plan {
public:
    GoalType goal_;
    std::vector<Step> steps_;

};

}} // ns ai::bm
