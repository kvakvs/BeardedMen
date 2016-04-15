#include "game/co_brains.h"
#include "game/world.h"

namespace bm {

void BrainsComponent::think(const World &w) {
    if (w.have_orders() == false) {
        return;
    }
}

} // ns bm
