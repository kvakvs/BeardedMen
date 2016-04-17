#include "game/co_brains.h"
#include "game/world.h"
#include "ai/plan.h"

namespace bm {

void BrainsComponent::think() {
    while (not desires_.empty()) {
        auto &pair = desires_.back();
        auto &ctx = pair.second;
        if (ctx.world_->conditions_stand_true(pair.first, ctx)) {
            // we do not desire anymore that which came true
            desires_.pop_back();
        } else {
            break;
        }
    }
    if (desires_.empty()) {
        // Idle, no desires. Wait. Loiter. Drink alcohol.
        return;
    }

    qDebug() << "Picking up one own desire to plan";

    auto one_desire = desires_.front();
    auto &want = one_desire.first;

    ai::Context ctx = one_desire.second; // copy
    ctx.actor_ = get_parent();

    auto actions = ai::propose_plan(
                ctx.world_->get_current_situation(want, ctx),
                want,   // want
                ctx);   // context

    // TODO: if there is no plan of actions, drop the desire
    // TODO: Mark as ignored for a short time
}

void BrainsComponent::want(const ai::MetricContextPair &desire) {
    desires_.push_back(desire);
}

} // ns bm
