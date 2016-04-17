#include "game/co_brains.h"
#include "game/world.h"
#include "ai/plan.h"
#include "ai/action.h"

namespace bm {

void BrainsComponent::think() {
    if (not wish_.current.has_value()) {
        pick_and_plan();
    }
    if (wish_.current.has_value()) {
        follow_the_plan();
    }
}

void BrainsComponent::pick_and_plan() {
    while (not wish_.desires.empty()) {
        auto &pair = wish_.desires.back();

        ai::Context ctx = pair.second;
        ctx.actor_ = get_parent();
        if (ctx.world_->conditions_stand_true(pair.first, ctx)) {
            // we do not desire anymore that which came true
            wish_.desires.pop_back();
        } else {
            break;
        }
    }
    if (wish_.desires.empty()) {
        // Idle, no desires. Wait. Loiter. Drink alcohol.
        return;
    }

    auto one_desire = wish_.desires.front();
    auto &want = one_desire.first;

    ai::Context ctx = one_desire.second; // copy
    ctx.actor_ = get_parent();

    auto actions = ai::propose_plan(
                ctx.world_->get_current_situation(want, ctx),
                want,   // want
                ctx);   // context

    // if there is no plan of actions, we don't want it anymore
    if (actions.empty()) {
        // forget this one
        wish_.desires.erase(wish_.desires.begin());
        wish_.current = {};
        return;
    }

    qDebug() << "brains: have plan!";
    wish_.current = one_desire;

    // High level action plan only contains actions. Now try and use context
    // to plan actual activities with destination objects and positions
    ai::flesh_out_a_plan(/*out*/ wish_.plan, actions, ctx);
}

void BrainsComponent::follow_the_plan()
{
    if (wish_.plan.empty()) {
        return; // nothing to do
    }

    qDebug() << "brains: follow plan";

    auto& step = wish_.plan.front();
    switch (step.action_) {
    case ai::ActionType::None: // no action (this should not be in plan either)
        wish_.plan.erase(wish_.plan.begin());
        return;
    case ai::ActionType::Mine: {
            auto dst = step.arg_.get_pos();
            auto ent = get_parent()->as_entity();
            if (adjacent_or_same(ent->get_pos(), dst)) {
                ComponentObject::get_world()->mine_voxel(dst);
            }
            // Finish step even if mining failed
            wish_.plan.erase(wish_.plan.begin());
        } break;
    case ai::ActionType::Move: {
            auto dst = step.arg_.get_pos();
            auto ent = get_parent()->as_entity();
            if (not ent->is_moving() || ent->get_move_destination() != dst) {
                ent->move_to(dst);
            } else if (adjacent_or_same(ent->get_pos(), dst)) {
                // Move finished
                wish_.plan.erase(wish_.plan.begin());
            }
            // else keep trying to replan move until this wish is dropped
        } break;
    }
}

void BrainsComponent::want(const ai::MetricContextPair &desire) {
    wish_.desires.push_back(desire);
}


} // ns bm
