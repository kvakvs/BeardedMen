#pragma once

#include "game/component.h"
#include "ai/goal.h"
#include "util/optional.h"

namespace bm {

class World;

//enum class Strategy: uint8_t {
//    Neutral,
//    Aggressive,
//    Defensive,
//    Panic
//};

// TODO: evict from this file, make own file
class BrainsComponent {
public:
    BrainsComponent(ComponentObject *p): parent_(p) {}
    ComponentObject* get_parent() const { return parent_; }

    // -- Intelligent --
    void think();

    // Set a goal. AI will try and find a way to reach the goal.
    void want(const ai::MetricContextPair &desire);

private:
    // From our desires pick one (first) and try find a plan for it.
    // If plan is not possible, the desire is dropped
    void pick_and_plan();
    void follow_the_plan();

private:
    ComponentObject *parent_;

    // List of all things we want done, plan how we want it done and list of
    // specific actions
    struct {
        std::vector<ai::MetricContextPair> desires;
        Optional<ai::MetricContextPair> current;
        // steps with arguments
        ai::Activities plan;
    } wish_;
};

} // namespace bm
