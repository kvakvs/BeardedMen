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
};

// Decision search node, represents a step in solution
// Distance is a metric of how many conditions are still not satisfied
class AstarNode {
public:
    ActionType action_;
    MetricVec metrics_;
    const AstarGlobalState *state_;

    AstarNode() = default;
    AstarNode(const MetricVec& m, const AstarGlobalState* st)
        : metrics_(m), state_(st) {}

    float GoalDistanceEstimate(AstarNode& goal) {
        // Assume goal.state_ and our state_ are same size, same metrics
        Q_ASSERT(metrics_.size() == goal.metrics_.size());
        int result = 0;
        for (int i = 0; i < metrics_.size(); ++i) {
            // not== because there is no !=
            if (not (metrics_[i] == goal.metrics_[i])) {
                result++;
            }
        }
        return result;
    }

    bool IsGoal(AstarNode& goal) {
        return goal.metrics_ == metrics_;
    }

    bool GetSuccessors(AStarSearch<AstarNode>* astarsearch,
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
            adef.apply_to(new_metrics);

            // create new state with this action effects
            AstarNode next_n(new_metrics, state_);
            astarsearch->AddSuccessor(next_n);
        }
        return true;
    }

    float GetCost(AstarNode& successor) { return 1.0f; }

    bool IsSameState(AstarNode& rhs) {
        return metrics_ == rhs.metrics_;
    }

    //void PrintNodeInfo() {}
};

ActionVec propose_plan(const MetricVec& from_c0,
                       const MetricVec& to_c0,
                       const World& wo, ComponentObject* co)
{
    using AstarEngine = AStarSearch<AstarNode>;
    AstarEngine search_engine;

    // For each actiondef in all actions - and for each requirement in each
    // actiondef - see if we need to add another metric from the world.
    MetricVec from_c(from_c0);
    MetricVec to_c(to_c0);
    AstarGlobalState glob_state { to_c, co->ai_get_all_actions() };

    for (auto& adef: glob_state.available_actions_) {
        for (auto& req: adef.requires_) {
            if (not impl::have_metric(from_c, req.type_)) {
                auto current = wo.read_metric(req, co);
                from_c.push_back(current);
                to_c.push_back(current);
                break;
            }
        }
    }
    qDebug() << "from_c" << from_c;
    qDebug() << "to_c" << to_c;

    AstarNode from(from_c, &glob_state);
    AstarNode to(to_c, &glob_state);
    search_engine.SetStartAndGoalStates(from, to);

    unsigned int search_state;
    unsigned int search_steps = 0;

    do {
        search_state = search_engine.SearchStep();
        search_steps++;
    } while (search_state == AstarEngine::SEARCH_STATE_SEARCHING);

    ActionVec plan;

    if (search_state == AstarEngine::SEARCH_STATE_SUCCEEDED) {
        qDebug() << "Search found goal state\n";

        plan.reserve(search_steps);
        for (AstarNode* node = search_engine.GetSolutionStart();
             node; node = search_engine.GetSolutionNext())
        {
            plan.push_back(node->action_);
            qDebug() << "Solution step " << (int)node->action_ << endl;
        }

        // Once you're done with the solution you can free the nodes up
        search_engine.FreeSolutionNodes();
    } else {
        if (search_state == AstarEngine::SEARCH_STATE_FAILED) {
            qDebug() << "Search terminated. Did not find goal state\n";
        }
    }

    search_engine.EnsureMemoryFreed();
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
