#pragma once

#include "ai/goal.h"
#include "ai/action.h"

namespace bm {

class World;
class ComponentObject;

namespace ai {

ActionVec propose_plan(const MetricVec& from,
                       const MetricVec& to,
                       const World &wo,
                       ComponentObject *co);

} // ns bm::ai
} // ns bm
