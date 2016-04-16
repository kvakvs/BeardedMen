#pragma once

#include <vector>

#include "ai/goal.h"

namespace bm {namespace ai {

enum class ActionType: uint16_t {
    None,
    Walk,
    Mine,
};

// Defines an action and its effects. Use only ActionType for planning
class ActionDef {
public:
    ActionType type_;
    // Preconditions
    ai::MetricVec requires_;
    // Effect that you get using this command
    ai::MetricVec gives_;

    explicit ActionDef(ActionType t,
                       const ai::MetricVec& requires,
                       const ai::MetricVec& gives)
        : type_(t), requires_(requires), gives_(gives) {}
};

using ActionVec = std::vector<ai::ActionType>;
using ActionDefVec = std::vector<ai::ActionDef>;

}} // ns bm::ai
