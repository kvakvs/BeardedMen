#pragma once

//#include <stdint.h>
//#include "vector.h"
//#include "gfx/model.h"
//#include "model_id.h"
//#include "game/component.h"
#include "game/ai_goal.h"

namespace bm {

class World;

// TODO: evict from this file, make own file
class BrainsComponent {
public:
    // -- Intelligent --
    void think(const World &w);

    // Set a goal. AI will try and find a way to reach the goal.
    void want(const ai::Condition &desire) {
        ai_want_.push_back(desire);
    }

private:
    //int ai_goal_; // goal index or -1

    // List of all things we want done
    std::vector<ai::Condition> ai_want_;

    //std::vector<ai::Step> ai_steps_;
    //ai::State ai_state_;
};

} // namespace bm
