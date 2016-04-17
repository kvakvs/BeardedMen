#pragma once

//#include <stdint.h>
//#include "util/vec.h"
//#include "gfx/model.h"
//#include "model_id.h"
#include "game/component.h"
#include "ai/goal.h"

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
    void think();

    // Set a goal. AI will try and find a way to reach the goal.
    void want(const ai::MetricContextPair &desire);

private:
    // List of all things we want done
    std::vector<ai::MetricContextPair> desires_;
};

} // namespace bm
