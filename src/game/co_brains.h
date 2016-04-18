#pragma once

#include "game/component.h"
#include "ai/goal.h"
#include "util/optional.h"
#include "game/g_defs.h"

namespace bm {

class World;

// TODO: evict from this file, make own file
class BrainsComponent {
public:
    BrainsComponent(AnimateObject *p): parent_(p) {}
    AnimateObject* get_parent() const { return parent_; }

    // -- Intelligent --
    void think();

    // Set a goal. AI will try and find a way to reach the goal.
    void want(ai::Order::Ptr order);
    bool is_idle() const { return not task_.current; }

private:
    AnimateObject *parent_;

    // List of all things we want done, plan how we want it done and list of
    // specific actions
    struct {
        //std::vector<ai::Order::Ptr> orders;
        ai::Order::Ptr current;
        // steps with arguments
        ai::Activities plan;
    } task_;

private:
    // From our desires pick one (first) and try find a plan for it.
    // If plan is not possible, the desire is dropped
    void build_plan();
    void follow_the_plan();
    // We consider plan finished or interrupted. Check if its finished, and
    // report back
    void finish_plan(PlanResult pr);
    void finish_plan_step();

    // This is called from switch-case, returns true requesting immediate return.
    // If returns false - perform case break
    bool plan_step_move(const Vec3i& dst, MovePrecision mp);

    // Use_pos is current entity pos. Change it if you reach to mine on a
    // different depth (above or below) because mine step checks adjacent cell
    // on the same depth.
    void plan_step_mine(const Vec3i& dst);
};

} // namespace bm
