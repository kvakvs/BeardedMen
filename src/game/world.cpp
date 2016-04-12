#include "game/world.h"

namespace bm {

void World::add(IEntity *ent) {
    ent->set_id(ent_id_);
    entities_[ent_id_++] = ent;
}

void World::think() {
    // Here we think for entities (passive things like gravity)
    each_ent([this](auto id, auto ent) {
        auto ent_pos = ent->get_pos();
        auto block_under = get_under(ent_pos);
        if (is_air(block_under)) {
            // fall
            ent_pos.setY(ent_pos.getY() + 1);
            ent->set_pos(ent_pos);
        }
    });

    // Entities think for themselves
    each_ent([this](auto id, auto ent) {
        auto i = dynamic_cast<IIntelligent*>(ent);
        if (i) {
            i->think(*this);
        }
    });
}

} // ns bm
