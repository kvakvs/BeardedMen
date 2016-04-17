#pragma once

#include <vector>

#include "ai/goal.h"

namespace bm {namespace ai {

enum class ActionType: uint16_t {
    None,
    Move,
    Mine,
};

// Actions cost for Astar planning
float get_action_cost(ActionType at);

QDebug operator<< (QDebug d, ActionType at);

// Defines an action and its effects. Use only ActionType for planning
class ActionDef {
public:
    ActionType action_;
    // Preconditions
    MetricVec requires_;
    // Effect that you get using this command
    MetricVec gives_;

    explicit ActionDef(ActionType t,
                       const MetricVec& requires,
                       const MetricVec& gives)
        : action_(t), requires_(requires), gives_(gives) {}

    // Overwrites readings of metrics from gives_ to state_
    void copy_readings(MetricVec& state) const;
};

QDebug operator<< (QDebug d, const ActionDef& ad);

using ActionVec = std::vector<ai::ActionType>;
using ActionDefVec = std::vector<ai::ActionDef>;


}} // ns bm::ai
