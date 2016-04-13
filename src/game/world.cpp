#include "game/world.h"

namespace bm {

void World::add(ComponentObject *co) {
    co->as_entity()->set_id(ent_id_);
    objects_[ent_id_++] = co;
}

void World::think() {
    // Here we think for entities (passive things like gravity)
    each_obj([this](auto /*id*/, auto co) {
        auto ent         = co->as_entity();
        auto ent_pos     = ent->get_pos();
        auto block_under = get_under(ent_pos);
        if (is_air(block_under)) {
            // fall
            ent_pos.setY(ent_pos.getY() + 1);
            ent->set_pos(ent_pos);
        }
    });

    // Entities think for themselves
    each_obj([this](auto /*id*/, auto co) {
        auto brains = co->as_brains();
        if (brains) {
            brains->think(*this);
        }
    });
}

} // ns bm
