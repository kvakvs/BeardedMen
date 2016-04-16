#include "game/co_brains.h"
#include "game/world.h"

namespace bm {

void BrainsComponent::think(const World &wo, ComponentObject *co) {
    if (ai_goals_.empty()) {
        return;
    }
    ai::Goal& goal = ai_goals_.front();
    auto actions = co->ai_choose_actions(goal);
}

void BrainsComponent::want(const ai::Goal &desire) {
    ai_goals_.push_back(desire);
}

} // ns bm
