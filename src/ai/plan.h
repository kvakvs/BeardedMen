#pragma once

#include "ai/goal.h"
#include "ai/action.h"

namespace bm {

class World;
class ComponentObject;

namespace ai {

ActionVec propose_plan(const MetricVec& from,
                       const MetricVec& to,
                       const ai::Context& ctx);

namespace impl {
    bool check_requirements(const MetricVec& required,
                            const MetricVec& have);
    bool have_metric(const MetricVec& v, MetricType mt);
    Metric get_metric(const MetricVec& v, MetricType mt);
} // ns impl

} // ns bm::ai
} // ns bm
