#pragma once

//#include <stdint.h>
//#include "vector.h"
//#include "gfx/model.h"
//#include "model_id.h"
//#include "game/component.h"

namespace bm {

class World;

// TODO: evict from this file, make own file
class BrainsComponent {
public:
    // -- Intelligent --
    void think(const World &w) {}
};

} // namespace bm
