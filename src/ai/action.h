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
    MetricVec requires_;
    // Effect that you get using this command
    MetricVec gives_;

    explicit ActionDef(ActionType t,
                       const MetricVec& requires,
                       const MetricVec& gives)
        : type_(t), requires_(requires), gives_(gives) {}

    // Overwrites metrics from gives_ in state_
    void apply_to(MetricVec& state) const;
};

using ActionVec = std::vector<ai::ActionType>;
using ActionDefVec = std::vector<ai::ActionDef>;

}} // ns bm::ai
