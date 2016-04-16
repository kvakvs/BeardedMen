#pragma once

#include <vector>
#include "ai/goal.h"

namespace bm {namespace ai {

enum class ActionType: uint16_t {
    Walk,
    Mine,
};

class Action {
public:
    ActionType type_;
    // Preconditions and effect
    ai::Goal intent_;

    explicit Action(ActionType t, const ai::Goal &g)
        : type_(t), intent_(g) {}
};

using ActionVec = std::vector<ai::Action>;

}} // ns bm::ai
