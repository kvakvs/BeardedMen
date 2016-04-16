#include "ai/plan.h"

#include "game/component.h"
#include "game/world.h"

#include <stlastar.h>

namespace bm {
namespace ai {

// Decision search node, represents a step in solution
// Distance is a metric of how many conditions are still not satisfied
class AstarNode {
public:
    MetricVec state_;
    const MetricVec* goal_ = nullptr;

    AstarNode() = default;
    AstarNode(const MetricVec& state, const MetricVec* goal)
        : state_(state), goal_(goal) {}

    float GoalDistanceEstimate(AstarNode& goal) {
        // Assume goal.state_ and our state_ are same size, same metrics
        Q_ASSERT(state_.size() == goal.state_.size());
        int result = 0;
        for (int i = 0; i < state_.size(); ++i) {
            // not== because there is no !=
            if (not (state_[i] == goal.state_[i])) {
                result++;
            }
        }
        return result;
    }

    bool IsGoal(AstarNode& goal) {
        return goal.state_ == state_;
    }

    bool GetSuccessors(AStarSearch<AstarNode>* astarsearch,
                       AstarNode* parent_node)
    {
        // For each action we have try model new state, and push it to search
        for (auto& metric: state_) {

        }
        return true;
    }

    float GetCost(AstarNode& successor) { return 1.0f; }

    bool IsSameState(AstarNode& rhs) {
        return state_ == rhs.state_;
    }

    //void PrintNodeInfo() {}
};

ActionVec propose_plan(const MetricVec& from_c,
                       const MetricVec& to_c,
                       const World& wo, ComponentObject* co)
{
    AStarSearch<AstarNode> astarsearch;
    AstarNode from(from_c, &to_c);
    AstarNode to(to_c, &to_c);
    astarsearch.SetStartAndGoalStates(from, to);

    unsigned int search_state;
    unsigned int search_steps = 0;

    do {
        search_state = astarsearch.SearchStep();
        search_steps++;
    } while (search_state == AStarSearch<AstarNode>::SEARCH_STATE_SEARCHING);

    if (search_state == AStarSearch<AstarNode>::SEARCH_STATE_SUCCEEDED) {
        cout << "Search found goal state\n";

        int steps = 0;

        for (AstarNode* node = astarsearch.GetSolutionStart();
             node;
             node = astarsearch.GetSolutionNext())
        {
        }

        cout << "Solution steps " << steps << endl;

        // Once you're done with the solution you can free the nodes up
        astarsearch.FreeSolutionNodes();

    } else {
        if (search_state == AStarSearch<AstarNode>::SEARCH_STATE_FAILED) {
            cout << "Search terminated. Did not find goal state\n";
        }
    }

    // Display the number of loops the search went through
    cout << "SearchSteps : " << search_steps << "\n";
    astarsearch.EnsureMemoryFreed();

    return {};
}

}  // ns bm::ai
}  // ns bm
