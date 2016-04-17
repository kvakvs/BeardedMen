#include "ai/plan.h"

#include "game/component.h"
#include "game/world.h"

#include <stlastar.h>

namespace bm {
namespace ai {

// Single copy of global search state. Used in one search for all nodes.
class AstarGlobalState {
public:
    MetricVec goal_;
    const ActionDefVec& available_actions_;
    // Astar will stop if path has one step without creating any solution nodes.
    // We need to record that single step here so it is not lost.
    std::vector<ActionType> single_step_;
};

// Decision search node, represents a step in solution
// Distance is a metric of how many conditions are still not satisfied
class AstarNode {
public:
    ActionType action_;     // What to do in this step
    MetricVec metrics_;     // What the world will become
    AstarGlobalState *state_;

    AstarNode() = default;
    AstarNode(const AstarNode &) = delete;
    AstarNode(AstarNode &&) = default;
    AstarNode(const MetricVec& m, AstarGlobalState* st, ActionType at)
        : metrics_(m), state_(st), action_(at) {}

    AstarNode& operator= (AstarNode &&) = default;

    float GoalDistanceEstimate(AstarNode& goal) {
        // Assume goal.state_ and our state_ are same size, same metrics
        int result = 0;
        for (int i = 0; i < goal.metrics_.size(); ++i) {
            auto current = impl::get_metric(metrics_, goal.metrics_[i].type_);

            // not== because there is no !=
            if (not (current == goal.metrics_[i])) {
                result++;
            }
        }
        //qDebug() << "Goal distance estimate" << result;
        return result;
    }

    bool IsGoal(AstarNode& goal) {
//        for(auto &goal_mtr: goal.metrics_) {
//            auto current = impl::get_metric(metrics_, goal_mtr.type_);
//            if (not (goal_mtr == current)) { // do not have operator!=
//                return false;
//            }
//        }
//        return true;
        return impl::check_requirements(goal.metrics_, metrics_);
    }

    bool GetSuccessors(AStarSearch<AstarNode>* engine,
                       AstarNode* /*parent_node*/)
    {
        // For each action we have try model new state, and push it to search
        for (const ActionDef &adef: state_->available_actions_) {
            // if action precondition on current state_ is fulfilled
            if (not impl::check_requirements(adef.requires_, metrics_)) {
                continue;
            }

            // (todo: missing metrics should be prepared before search)
            MetricVec new_metrics(metrics_);
            adef.copy_readings(new_metrics);
            //qDebug() << "Propose action" << adef.action_ << ": " << new_metrics;

            // Astar will stop if solution has only one step. If this is the
            // case - we record this proposed step in the state
            if (impl::check_requirements(state_->goal_, new_metrics)) {
                state_->single_step_.push_back(adef.action_);
            }

            // create new state with this action effects
            AstarNode next_n(new_metrics, state_, adef.action_);
            engine->AddSuccessor(next_n);
        }
        return true;
    }

    float GetCost(AstarNode& successor) {
        return get_action_cost(successor.action_);
    }

    bool IsSameState(AstarNode& rhs) {
        return metrics_ == rhs.metrics_;
    }

    //void PrintNodeInfo() {}
};

ActionVec propose_plan(const MetricVec& from_c0,
                       const MetricVec& to_c0,
                       const ai::Context& ctx)
{
    using AstarEngine = AStarSearch<AstarNode>;
    AstarEngine engine;

    // For each actiondef in all actions - and for each requirement in each
    // actiondef - see if we need to add another metric from the world.
    MetricVec from_c(from_c0);
    MetricVec to_c(to_c0);

    Q_ASSERT(ctx.actor_);
    AstarGlobalState glob_state { {}, ctx.actor_->ai_get_all_actions() };
    auto wo = ComponentObject::get_world();

    for (auto& adef: glob_state.available_actions_) {
        //qDebug() << "Each avail action:" << adef;
        for (auto& req: adef.requires_) {
            if (not impl::have_metric(from_c, req.type_)) {
                auto extra_mtr = wo->read_metric(req, ctx);
                //qDebug() << "Extra metric:" << extra_mtr;
                from_c.push_back(extra_mtr);
                //to_c.push_back(extra_mtr);
            }
        }
    }
//    qDebug() << "HAVE" << from_c;
//    qDebug() << "WANT" << to_c;

    glob_state.goal_ = to_c; // to_c is probably changed - update
    AstarNode from(from_c, &glob_state, ActionType::None);
    AstarNode to(to_c, &glob_state, ActionType::None);
    engine.SetStartAndGoalStates(from, to);

    unsigned int search_state;
    unsigned int search_steps = 0;

    do {
        search_state = engine.SearchStep();
        search_steps++;
    } while (search_state == AstarEngine::SEARCH_STATE_SEARCHING);

    ActionVec plan;

    if (search_state == AstarEngine::SEARCH_STATE_SUCCEEDED) {
        //qDebug() << "plan: Found a plan";
        plan.reserve(search_steps);
        for (AstarNode* node = engine.GetSolutionStart();
             node; node = engine.GetSolutionNext())
        {
            if (node->action_ != ActionType::None) {
//                qDebug() << "plan: Step=" << node->action_;
                plan.push_back(node->action_);
            }
        }
        if (not glob_state.single_step_.empty()) {
            auto extra_action = glob_state.single_step_.front();
//            qDebug() << "plan: [extra] Step=" << extra_action;
            plan.push_back(extra_action);
        }
        // Once you're done with the solution you can free the nodes up
        engine.FreeSolutionNodes();
    } else {
        if (search_state == AstarEngine::SEARCH_STATE_FAILED) {
            qDebug() << "plan: Did not find a plan";
        }
    }

    engine.EnsureMemoryFreed();
    return plan;
}

bool impl::check_requirements(const MetricVec &required, const MetricVec &have)
{
    for (auto& mtr: required) {
        // not ==, don't have operator!=
        if (not (get_metric(have, mtr.type_) == mtr)) {
            return false;
        }
    }
    return true;
}

Metric impl::get_metric(const MetricVec &v, MetricType mt)
{
    for (auto& mtr: v) {
        if (mtr.type_ == mt) {
            return mtr;
        }
    }
    Q_ASSERT(false);
}

bool impl::have_metric(const MetricVec &v, MetricType mt)
{
    for (auto& mtr: v) {
        if (mtr.type_ == mt) {
            return true;
        }
    }
    return false;
}

}  // ns bm::ai
}  // ns bm
