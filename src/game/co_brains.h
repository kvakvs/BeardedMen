#pragma once

//#include <stdint.h>
//#include "vector.h"
//#include "gfx/model.h"
//#include "model_id.h"
//#include "game/component.h"
#include "game/ai_goal.h"

namespace bm {

class World;

//enum class Strategy: uint8_t {
//    Neutral,
//    Aggressive,
//    Defensive,
//    Panic
//};

// TODO: evict from this file, make own file
class BrainsComponent {
public:
    // -- Intelligent --
    void think(const World &w);

    // Set a goal. AI will try and find a way to reach the goal.
    void want(const ai::Goal &desire);

private:

    // List of all things we want done
    std::vector<ai::Goal> ai_goals_;
};

} // namespace bm
