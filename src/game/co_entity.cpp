#include "game/co_entity.h"

#include <PolyVox/AStarPathfinder.h>

#include "game/world.h"

namespace bm {

void EntityComponent::step() {
    if (movement_.planned_route.empty() == false) {
        if (attempt_move(movement_.planned_route.front())) {
            movement_.planned_route.pop_front();
        } else {
            movement_.planned_route.clear();
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

// Relaxed validator which ignores walls for routing
inline bool voxel_validate_relaxed(const VolumeType* vol,
                                   const Vec3i &p) {
    return is_solid(vol->getVoxel(p + Vec3i(0,1,0)));
}

// Stricter validator which respects walls
static inline bool voxel_validate_strict(const VolumeType* vol,
                                         const Vec3i &p) {
    return is_air(vol->getVoxel(p)) &&
           is_solid(vol->getVoxel(p + Vec3i(0,1,0)));
}

bool EntityComponent::move_to(const Vec3i &dst)
{
    clear_planned_route();
    // TODO: Try first sides closer to the worker
    if (find_and_set_strict_route(dst)) return true;
    if (find_and_set_strict_route(dst + Vec3i(1,0,0))) return true;
    if (find_and_set_strict_route(dst + Vec3i(-1,0,0))) return true;
    if (find_and_set_strict_route(dst + Vec3i(0,0,1))) return true;
    if (find_and_set_strict_route(dst + Vec3i(0,0,-1))) return true;
    return false;
}

bool EntityComponent::move_to(const Vec3i &dst, MovePrecision mp)
{
    clear_planned_route();
    switch (mp) {
    case MovePrecision::Exact: return find_and_set_strict_route(dst);
    case MovePrecision::Adjacent:
        // TODO: Try first sides closer to the worker
        return find_and_set_strict_route(dst + Vec3i( 1, 0, 0))
            || find_and_set_strict_route(dst + Vec3i(-1, 0, 0))
            || find_and_set_strict_route(dst + Vec3i( 0, 0, 1))
            || find_and_set_strict_route(dst + Vec3i( 0, 0,-1));
    case MovePrecision::AdjacentDepth:
        // TODO: Try first sides closer to the worker
        return find_and_set_strict_route(dst + Vec3i( 1, 0, 0))
            || find_and_set_strict_route(dst + Vec3i(-1, 0, 0))
            || find_and_set_strict_route(dst + Vec3i( 0, 0, 1))
            || find_and_set_strict_route(dst + Vec3i( 0, 0,-1))
            || find_and_set_strict_route(dst + Vec3i( 1,-1, 0))
            || find_and_set_strict_route(dst + Vec3i(-1,-1, 0))
            || find_and_set_strict_route(dst + Vec3i( 0,-1, 1))
            || find_and_set_strict_route(dst + Vec3i( 0,-1,-1));
    }
}

bool EntityComponent::find_and_set_strict_route(const Vec3i& dst)
{
    // You are not going into the rock
    auto wo = ComponentObject::get_world();
    if (not is_air(wo->get_voxel(dst))) {
            return false;
    }

    Route path = find_route(dst);

    if (path.empty()) {
        return false;
    } else {
        set_planned_route(dst, path);
        return true;
    }
}

/*
void EntityComponent::move_to(const Vec3i &dst)
{
    clear_planned_route();
    Route relaxed_path = find_relaxed_route(dst);

    if (relaxed_path.empty()) {
        // No route, even ignoring walls
        return;
    } else {
        Route strict_path = find_route(dst);
        if (strict_path.empty()) {
            if (relaxed_path.size() > 1) {
                // Try one step back
                relaxed_path.pop_back();
                strict_path = find_route(relaxed_path.back());
                set_planned_route(dst, strict_path);
                return;
            }
            if (strict_path.empty()) {
                clear_planned_route();
                return;
            }
        }
        set_planned_route(dst, strict_path);
    }
}
*/

void EntityComponent::set_planned_route(const Vec3i &dst, Route &r) {
    movement_.dst = dst;
    movement_.planned_route = std::move(r);
}

Route EntityComponent::find_relaxed_route(const Vec3i &dst)
{
    Route result;
    auto vol = ComponentObject::get_world()->get_volume();

    // First find relaxed path over ground ignoring the walls
    pv::AStarPathfinderParams<VolumeType> pfpar(
                const_cast<VolumeType*>(vol),
                get_pos(), dst, & result,
                1.1 /*bias*/, 1000 /*maxlength*/,
                pv::SixConnected,
                & voxel_validate_relaxed);

    pv::AStarPathfinder<VolumeType> pf(pfpar);
    try {
        pf.execute();
        return result;
    } catch (std::runtime_error) {
        return Route();
    }
}

Route EntityComponent::find_route(const Vec3i &dst)
{
    Route result;
    auto vol = ComponentObject::get_world()->get_volume();

    // First find relaxed path over ground ignoring the walls
    pv::AStarPathfinderParams<VolumeType> pfpar(
                const_cast<VolumeType*>(vol),
                get_pos(), dst, & result,
                1.0 /*bias*/, 50 /*maxlength*/,
                pv::SixConnected,
                & voxel_validate_strict);

    pv::AStarPathfinder<VolumeType> pf(pfpar);
    try {
        pf.execute();
        return result;
    } catch (std::runtime_error) {
        return Route();
    }
}

QDebug operator<<(QDebug d, const EntityComponent &ent)
{
    d.nospace() << "Ent{";
    d << ent.get_id() << ";" << ent.get_pos();
    d << "}";
    return d;
}

QDebug operator<<(QDebug d, const Route &r)
{
    d.nospace() << "Route[";
    for (auto step: r) {
        d << step << "; ";
    }
    d << "]";
    return d;
}

} // ns bm
