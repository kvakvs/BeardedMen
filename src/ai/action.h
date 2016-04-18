#pragma once

#include <vector>

#include "ai/goal.h"
#include "tab/t_action.h"

namespace bm {
namespace ai {

// Defines an action and its effects. Use only ActionType for planning
// Has preconditions (requires_) and effects (gives_).
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

// Actual thing to do with specific coords or a target
class Activity {
public:
    ActionType action_;
    Value      arg_;
};

using Activities = std::vector<Activity>;

// Flesh out plans using action list and context (with destination)
void flesh_out_a_plan(Activities& out_plan,
                      const ActionVec& actions,
                      const Context& ctx);

} // ns bm::ai
} // ns bm
