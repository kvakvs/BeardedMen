#include "game/co_entity.h"

#include <PolyVox/AStarPathfinder.h>

#include "game/world.h"

namespace bm {

void EntityComponent::step() {
    if (movement.planned_route_.empty() == false) {
        if (attempt_move(movement.planned_route_.front())) {
            movement.planned_route_.pop_front();
        } else {
            movement.planned_route_.clear();
        }
    }
}

bool EntityComponent::attempt_move(const Vec3i &new_pos)
{
    auto wo = ComponentObject::get_world();
    if (not is_solid(wo->get_voxel(new_pos))) {
        set_pos(new_pos);
        return true;
    }
    return false;
}

static inline bool astar_vox_validator(const VolumeType*, const Vec3i&) {
    return true;
}

void EntityComponent::move_to(const Vec3i &pos)
{
    std::list<Vec3i> path;
    auto vol = ComponentObject::get_world()->get_volume();
    pv::AStarPathfinderParams<VolumeType> pfpar(
                const_cast<VolumeType*>(vol),
                get_pos(), pos,
                &path,
                1.0,
                1000, // maxlength?
                pv::SixConnected, // can only walk throught the side, no diagonals
                & astar_vox_validator);

    pv::AStarPathfinder<VolumeType> pf(pfpar);
    try {
        pf.execute();
        set_planned_route(pos, path);
    } catch (std::runtime_error) {
        clear_planned_route();
    }
}

} // ns bm
