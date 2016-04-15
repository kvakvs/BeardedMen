#include "game/world.h"
#include "game/co_worker.h"
#include "game/co_brains.h"

#include <QDebug>

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
        // if entity has planned route
        ent->step(*this);
    });

    // Entities think for themselves
    each_obj([this](auto id, auto co) {
        BrainsComponent* brains = co->as_brains();
        if (brains) {
            brains->think(*this);
        } // if brains

//        WorkerComponent* worker = co->as_worker();
//            if (co->is_idle()) {
//                for (Order::Ptr order: orders_)
//                {
//                    if (worker->take_order(co, order)) {
//                        qDebug() << "Order accepted (removed from queue)";
//                        orders_.erase(order);
//                        break;
//                    } // if order taken
//                } // for each order
//            } else {
//                // not idle, have order
//                worker->perform(*this, co);
//            }
//        } // if worker
    });
}

bool World::is_mineable(const Vec3i &pos) const {
    auto vox = volume_.getVoxel(pos);
    return is_solid(vox);
}

void World::mine_voxel(const Vec3i &pos) {
    if (is_solid(volume_.getVoxel(pos))) {
        volume_.setVoxel(pos, VoxelType());
        any_voxel_changed_ = true;
        // TODO: Produce a drop with mined rock
    } else {
        qDebug() << "can't mine - not solid";
    }
}

//void World::add_position_order(const Vec3i &pos, JobType jt) {
//    orders_.insert(std::make_shared<PositionOrder>(pos, jt));
//}

} // ns bm
