#include "game/world.h"
#include "game/co_worker.h"
#include "game/co_brains.h"
#include "game/co_body.h"

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
            brains->think(*this, co);

            ai::MetricVec g = get_random_desire();
            // Now he wants to do the order, unless it's impossible
            brains->want(g);
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

bool World::add_goal(const ai::MetricVec& desired) {
    //if (not conditions_stand_true(goal, subject)) {
        // World goals don't check preconditions, so you can wish all you want
        // Actual workers DO check preconditions, so there may be no 1 to do it
    desired_changes_.push_back(desired);
    return true;
//    }
//    return false;
}

ai::MetricVec World::get_random_desire() {
    // Pick a random order. Check if it is not fulfilled yet. Give out.
    while (not desired_changes_.empty()) {
        std::uniform_int_distribution<size_t> rand_id(0, desired_changes_.size());
        size_t oid = rand_id(rand_);

        ai::MetricVec some_desire = desired_changes_[oid];

//        if (some_desire.is_fulfilled(*this)) {
//            // definitely fulfilled - forget it
//            desired_changes_.erase(desired_changes_.begin() + oid);
//        }
        // not fulfilled or probably not fulfilled
        return some_desire;
    }
    return ai::MetricVec();
}

void World::add_mining_goal(const Vec3i &pos)
{
    ai::MetricVec g { ai::Metric(ai::MetricType::BlockIsNotSolid, pos) };
    if (add_goal(g)) {
        qDebug() << "Player wishes to mine out a block";
    } else {
        qDebug() << "Can't mine - there is no rock";
    }
}

bool World::conditions_stand_true(const ai::MetricVec &cond,
                                  const ComponentObject* subject) const
{
    for (auto& mtr: cond) {
        // not ==, don't have operator!=
        if (not (read_metric(mtr, subject) == mtr)) {
            return false;
        }
    }
    return true;
}

ai::MetricVec World::get_current_situation(const ai::MetricVec &desired,
                                           const ComponentObject* subject) const
{
    ai::MetricVec result;
    for (auto &mtr: desired) {
        result.push_back(read_metric(mtr, subject));
    }
    return result;
}

ai::Metric World::read_metric(const ai::Metric &metric,
                              const ComponentObject* subject) const
{
    auto mt = metric.type_;

    switch (mt) {
    case ai::MetricType::NearPosition: {
            auto ent = subject->as_entity();
            if (not ent) { return ai::Metric(mt); }
            auto pos = ent->get_pos();
            return ai::Metric(mt, pos);
        } break;

    case ai::MetricType::HaveHand: {
            auto bo = subject->as_body();
            if (not bo) { return ai::Metric(mt); }
            auto has_hand = bo->has_body_part(BodyComponent::PartType::Hand);
            return ai::Metric(mt, has_hand);
        } break;

    case ai::MetricType::HaveLeg: {
            auto bo = subject->as_body();
            if (not bo) { return ai::Metric(mt); }
            auto has_leg = bo->has_body_part(BodyComponent::PartType::Leg);
            return ai::Metric(mt, has_leg);
        } break;

    case ai::MetricType::HaveMiningPick: {
            return ai::Metric(mt, true);
        } break;

    case ai::MetricType::BlockIsNotSolid: {
            // air or liquid will satisfy the condition
            auto vox = get_voxel(metric.arg_.get_pos());
            return ai::Metric(mt, not bm::is_solid(vox));
        } break;
    }
}

//void World::add_position_order(const Vec3i &pos, JobType jt) {
//    orders_.insert(std::make_shared<PositionOrder>(pos, jt));
//}

} // ns bm
