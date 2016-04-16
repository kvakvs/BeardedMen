#include "game/co_worker.h"

#if 0

#include "game/co_body.h"
#include "game/co_brains.h"
#include "game/world.h"

#include "world_pager.h"

#include <PolyVox/AStarPathfinder.h>
#include <QDebug>

namespace bm {

bool WorkerComponent::take_order(ComponentObject* co,
                                 Order::Ptr order)
{
    // Entity used for navigation
    EntityComponent* ent = co->as_entity();
    Q_ASSERT(ent); // worker that is not entity is an error

    // Body used to check if the work can be done
    // Now all jobs require one hand at least (to hold tool or weapon)
    BodyComponent* body = co->as_body();
    Q_ASSERT(body);
    if (not body->can_perform_job(order->get_job_type())) {
        qDebug() << "Body cannot perform job";
        return false;
    }

    //order_ = order;

    // Define the intent: we want to have block mined
    BrainsComponent *brains = co->as_brains();

    switch (order->get_target_type()) {
    case OrderTargetType::Position: {
            auto po = (PositionOrder*)order.get();
            ai::Condition to_mine(ai::EffectType::BlockMined,
                                  ai::Check::IsTrue,
                                  po->get_pos());
            brains->want(to_mine);
        } break;
    default: qDebug() << "Unsupported order type";
    }

    return true;
}

void WorkerComponent::perform(World& wo, ComponentObject *co)
{
    if (is_idle()) {
        return;
    }
    switch (order_->get_target_type()) {
    case OrderTargetType::Position:
        perform_position_order(wo, co, (PositionOrder*)order_.get());
    }
}

static inline bool astar_vox_validator(const VolumeType*, const Vec3i&) {
    return true;
}

void WorkerComponent::perform_position_order(World& wo,
                                             ComponentObject *co,
                                             PositionOrder *po)
{
    // First: Check that we're close enough. Move to the destination.
    qDebug() << "Perform position order";

    // Prepare path just in case
    EntityComponent* ent = co->as_entity();
    bool is_adjacent = adjacent_or_same(ent->get_pos(), po->get_pos());

    if (ent->has_planned_route() == false && not is_adjacent)
    {
        plan_path_to(wo, po, ent);
    }

    if (is_adjacent) {
        // Reached destination and can work
        wo.mine_voxel(po->get_pos());
        // Complete
        order_.reset();
        ent->clear_planned_route(); // stop moving
    }
}

void WorkerComponent::plan_path_to(World& wo,
                                   PositionOrder *po,
                                   EntityComponent *ent) {
    {
        auto fr = ent->get_pos();
        auto to = po->get_pos();
        qDebug() << "astar from" << fr.getX() << fr.getY() << fr.getZ()
                 << "to" << to.getX() << to.getY() << to.getZ();
    }

    std::list<Vec3i> path;
    pv::AStarPathfinderParams<VolumeType> pfpar(
                wo.get_volume(),
                ent->get_pos(),
                po->get_pos(),
                &path,
                1.0,
                1000, // maxlength?
                pv::SixConnected,
                & astar_vox_validator);

    pv::AStarPathfinder<VolumeType> pf(pfpar);
    try {
        pf.execute();
        ent->set_planned_route(path);
    } catch (std::runtime_error) {
        // No path - cancel order
        order_.reset();
        return;
    }
}

} // ns bm

#endif//0
