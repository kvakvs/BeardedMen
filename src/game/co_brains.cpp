#include "game/co_brains.h"
#include "game/world.h"
#include "ai/plan.h"

namespace bm {

void BrainsComponent::think(const World &wo, ComponentObject *co) {
    if (desires_.empty()) {
        // Idle, no desires. Wait. Loiter. Drink alcohol.
        return;
    }
    ai::MetricVec& one_desire = desires_.front();

    auto actions = ai::propose_plan(
                wo.get_current_situation(one_desire, co), // have now
                one_desire,  // want
                wo, co);

    // TODO: if there is no plan of actions, drop the desire
    // TODO: Mark as ignored for a short time
}

void BrainsComponent::want(const ai::MetricVec &desire) {
    desires_.push_back(desire);
}

} // ns bm
