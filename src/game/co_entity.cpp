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

// Relaxed validator which ignores walls for routing
inline bool voxel_validate_relaxed(const VolumeType* vol,
                                       const Vec3i &p) {
    return is_solid(vol->getVoxel(p + Vec3i(0,1,0)));
}

// Stricter validator which respects walls
static inline bool voxel_validate_strict(const VolumeType* vol,
                                                       const Vec3i &p) {
    return is_air(vol->getVoxel(p));
           is_solid(vol->getVoxel(p + Vec3i(0,1,0)));
}

void EntityComponent::move_to(const Vec3i &dst)
{
    clear_planned_route();
    Route relaxed_path = find_relaxed_route(dst);

    if (relaxed_path.empty()) {
        qDebug() << "relaxed path empty";
        // No route, even ignoring walls
        return;
    } else {
        qDebug() << "relaxed path" << relaxed_path;
        Route strict_path = find_route(dst);
        if (strict_path.empty()) {
            if (relaxed_path.size() > 1) {
                // Try one step back
                relaxed_path.pop_back();
                strict_path = find_route(relaxed_path.back());
                qDebug() << "set route(2)" << strict_path;
                set_planned_route(dst, strict_path);
                return;
            }
            if (strict_path.empty()) {
                qDebug() << "strict path(2) empty";
                clear_planned_route();
                return;
            }
        }
        qDebug() << "set route(1)" << strict_path;
        set_planned_route(dst, strict_path);
    }
}

void EntityComponent::set_planned_route(const Vec3i &dst, Route &r) {
    movement.dst_ = dst;
    movement.planned_route_ = std::move(r);
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
                pv::EighteenConnected,
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
                1.0 /*bias*/, 1000 /*maxlength*/,
                pv::EighteenConnected,
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
