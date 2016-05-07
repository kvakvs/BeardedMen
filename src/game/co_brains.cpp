#include "game/co_brains.h"
#include "game/world.h"
#include "ai/plan.h"
#include "ai/action.h"
#include "game/g_defs.h"

namespace bm {

void BrainsComponent::think() {
//    if (not task_.current) {
//        pick_and_plan();
//    }
    if (task_.current) {
        follow_the_plan();
    }
}

void BrainsComponent::build_plan() {
    auto wo = AnimateObject::get_world();

//    // Drop order if is fulfilled
//    if (task_.current) {
//        auto ctx   = task_.current->ctx_;
//        ctx.actor_ = get_parent();

//        if (wo->conditions_stand_true(task_.current->desired_, ctx)) {
//            // we do not desire anymore that which came true
//            task_.current.reset();
//            return;
//        }
//    } else {
//        // Idle, no desires. Wait. Loiter. Drink alcohol.
//        return;
//    }

    auto &desired = task_.current->desired_;

    ai::Context ctx = task_.current->ctx_; // copy
    ctx.actor_ = get_parent();

    auto actions = ai::propose_plan(
                wo->get_current_situation(desired, ctx),
                desired,   // want
                ctx);   // context

    // if there is no plan of actions, we don't want it anymore
    if (actions.empty()) {
        // forget this one
        wo->report_impossible(task_.current->id_, PlanResult::NoPlan);
        task_.current = {};
        return;
    }

    // High level action plan only contains actions. Now try and use context
    // to plan actual activities with destination objects and positions
    ai::flesh_out_a_plan(/*out*/ task_.plan, actions, ctx);

    qDebug() << "brains: have plan!" << actions
             << " steps=" << task_.plan.size()
             << " planid=" << task_.current->id_;
}

void BrainsComponent::follow_the_plan()
{
    if (not task_.current || task_.plan.empty()) {
        return; // nothing to do
    }

    // if plan is fulfilled
    auto wo            = AnimateObject::get_world();
    ai::Order::Ptr ord = task_.current;
    ai::Context ctx    = ord->ctx_; // copy
    ctx.actor_         = get_parent();
    if (wo->conditions_stand_true(ord->desired_, ctx)) {
        // we do not desire anymore that which came true
        wo->report_fulfilled(ord->id_, PlanResult::Success);
        task_.current = {};
        task_.plan.clear();
        return;
    }

    Q_ASSERT(not task_.plan.empty());
    auto& step = task_.plan.front();
//    qDebug() << "brains: follow plan" << *get_parent()
//             << " step=" << step.action_;

    switch (step.action_) {
    case ai::ActionType::None: // no action (this should not be in plan either)
        finish_plan_step();
        return;
    case ai::ActionType::Mine: {
            auto dst = step.arg_.get_pos();
            plan_step_mine(dst);
        } break;
    case ai::ActionType::CreateRamp: {
            auto dst = step.arg_.get_pos();
            plan_step_create_ramp(dst);
        } break;
    case ai::ActionType::Move: {
            auto dst = step.arg_.get_pos();
            if (plan_step_move(dst, MovePrecision::Adjacent)) return;
        } break;
    case ai::ActionType::MoveClose: {
            auto dst = step.arg_.get_pos();
            if (plan_step_move(dst, MovePrecision::AdjacentDepth)) return;
        } break;
    }

    if (task_.plan.empty()) {
        finish_plan(PlanResult::Done);
    }
}

void BrainsComponent::plan_step_mine(const Vec3i& dst) {
    auto ent = get_parent()->as_entity();
    auto my_pos = ent->get_pos();

    // Mining rules: Can reach on same depth, or can reach 1 block down
    if (close_enough(my_pos, dst, MovePrecision::AdjacentDepth)) {
        AnimateObject::get_world()->mine_voxel(dst);
    }
    // Finish step even if mining failed
    finish_plan_step();
}

void BrainsComponent::plan_step_create_ramp(const Vec3i& dst) {
    auto ent = get_parent()->as_entity();
    auto my_pos = ent->get_pos();

    // Mining rules: Can reach on same depth, or can reach 1 block down
    if (close_enough(my_pos, dst, MovePrecision::AdjacentDepth)) {
        auto w = AnimateObject::get_world();
        auto vox = w->get_voxel(dst);
        vox.set_ramp(true);
        w->set_voxel(dst, vox);
        w->force_update_terrain_mesh_ = true;
    }
    // Finish step even if mining failed
    finish_plan_step();
}

bool BrainsComponent::plan_step_move(const Vec3i& dst, MovePrecision mp) {
    auto ent = get_parent()->as_entity();

    if (not close_enough(ent->get_move_destination(), dst, mp))
    {
        qDebug() << "move_to" << dst;
        if (not ent->move_to(dst, mp)) {
            // no route
            qDebug() << "no route" << ent->get_pos() << "to" << dst;
            finish_plan(PlanResult::MoveNoRoute);
            return true;
        }
        return false;
    }
    // Move finished
    auto close = close_enough(ent->get_pos(), dst, mp);
    if (close) {
        qDebug() << "plan: move finished";
        finish_plan_step();
        return false;
    }
    if (not ent->is_moving() && !close) {
        // not moving, but destination is correct - means can't move
        finish_plan(PlanResult::MoveStuck);
        return true;
    }
    return false;
}

void BrainsComponent::finish_plan(PlanResult pr)
{
    auto wo            = AnimateObject::get_world();
    ai::Order::Ptr ord = task_.current;
    ai::Context ctx    = ord->ctx_; // copy

    if (wo->conditions_stand_true(ord->desired_, ctx)) {
        wo->report_fulfilled(ord->id_, pr);
    } else {
        wo->report_failed(ord->id_, pr);
    }
    task_.current = {};
    task_.plan.clear();
}

void BrainsComponent::finish_plan_step()
{
    task_.plan.erase(task_.plan.begin());
}

void BrainsComponent::want(ai::Order::Ptr order) {
    task_.plan.clear();
    task_.current = order;
    build_plan();
}


} // ns bm
