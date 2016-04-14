#include "game/co_worker.h"
#include "game/co_body.h"
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
    if (! body->can_perform_job(order->get_job_type())) {
        qDebug() << "Body cannot perform job";
        return false;
    }

    order_ = order;
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

static bool astar_vox_validator(const VolumeType*, const Vec3i&) {
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
    if (path_.empty() && !adjacent_or_same(ent->get_pos(), po->get_pos())) {
        {
            auto fr = ent->get_pos();
            auto to = po->get_pos();
            qDebug() << "astar from" << fr.getX() << fr.getY() << fr.getZ()
                     << "to" << to.getX() << to.getY() << to.getZ();
        }
        pv::AStarPathfinderParams<VolumeType> pfpar(
                    wo.get_volume(),
                    ent->get_pos(),
                    po->get_pos(),
                    &path_,
                    1.0,
                    16,
                    pv::SixConnected,
                    & astar_vox_validator);
        //pfpar.maxNumberOfNodes = 16;
        //pfpar.connectivity = pv::EighteenConnected;

        pv::AStarPathfinder<VolumeType> pf(pfpar);
        try {
            pf.execute();
        } catch (std::runtime_error) {
            // No path - cancel order
            //order_.reset();
            //return;
        }

        for (auto v: path_) {
            qDebug() << v.getX() << v.getY() << v.getZ();
            //wo.get_volume()->setVoxel(v, VoxelType(BlockId::Clay, 1));
        }
    }
}


} // ns bm
