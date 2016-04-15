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
    void think(const World &w) {}

private:
    std::vector<ai::Condition> ai_goals_;
    std::vector<ai::Step> ai_steps_;
    //ai::State ai_state_;
};

} // namespace bm
