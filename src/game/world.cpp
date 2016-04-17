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
    sim_step_++;

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
        ent->step();
    });

    // Entities think for themselves
    each_obj([this](auto /*id*/, auto co) {
        BrainsComponent* brains = co->as_brains();
        if (brains) {
            brains->think();

            if (have_orders()) {
                auto dsr = get_random_desire(co);
                if (dsr) {
                    // Now he wants to do the order, unless it's impossible
                    brains->want(dsr);
                }
            }
        } // if brains
    });
}

bool World::is_mineable(const Vec3i &pos) const {
    auto vox = volume_.getVoxel(pos);
    return is_solid(vox);
}

void World::mine_voxel(const Vec3i &pos) {
    if (is_solid(volume_.getVoxel(pos))) {
        volume_.setVoxel(pos, VoxelType());
        force_update_terrain_mesh_ = true;
        // TODO: Produce a drop with mined rock
    } else {
        qDebug() << "can't mine - not solid";
    }
}

bool World::have_orders() const {
    return not orders_.empty()
            || not orders_low_.empty()
            || not orders_verylow_.empty();
}

bool World::add_order(ai::Order::Ptr desired) {
    orders_[desired->id_] = desired;
    return true;
}

void World::remove_order(ai::OrderId id)
{
    orders_.erase(id);
    orders_low_.erase(id);
    orders_verylow_.erase(id);
}

// Order scheduler
ai::Order::Ptr World::get_random_desire(ComponentObject *actor) {
    if (sim_step_ % VERY_LOW_PRIO_ORDERS_EVERY == 0) {
        qDebug() << sim_step_ << "very low prio";
        ai::Order::Ptr result = get_random_desire(actor, orders_verylow_);
        if (result) {
            return result;
        }
    }
    // Every N=5 steps try low prio orders, else return normal order if any
    if (sim_step_ % LOW_PRIO_ORDERS_EVERY == 0) {
        qDebug() << sim_step_ << "low prio";
        ai::Order::Ptr result = get_random_desire(actor, orders_low_);
        if (result) {
            return result;
        }
    }
    return get_random_desire(actor, orders_);
}

void World::add_mining_goal(const Vec3i &pos)
{
    ai::MetricVec m { ai::Metric(ai::MetricType::BlockIsNotSolid,
                                 ai::Value(pos),
                                 ai::Value(true)) };
    ai::Context ctx(nullptr);
    ctx.pos_ = pos;

    auto o = std::make_shared<ai::Order>(m, ctx);

    if (add_order(o)) {
        qDebug() << "Player wishes to mine out a block" << pos;
    }
}

void World::report_fulfilled(ai::OrderId id) {
    qDebug() << "world: Order" << id << "fulfilled";
    remove_order(id);
}

void World::report_failed(ai::OrderId id)
{
    lower_prio(id);
    qDebug() << "world: Order" << id << "failed";
}

void World::report_impossible(ai::OrderId id) {
    lower_prio(id);
    qDebug() << "world: Order" << id << "impossible";
}

bool World::conditions_stand_true(const ai::MetricVec &cond,
                                  const ai::Context& ctx) const
{
    for (auto& mtr: cond) {
        // not ==, don't have operator!=
        if (not (read_metric(mtr, ctx) == mtr)) {
            return false;
        }
    }
    return true;
}

ai::MetricVec World::get_current_situation(const ai::MetricVec &desired,
                                           const ai::Context& ctx) const
{
    ai::MetricVec result;
    for (auto &mtr: desired) {
        result.push_back(read_metric(mtr, ctx));
    }
    return result;
}

ai::Metric World::read_metric(const ai::Metric& metric,
                              const ai::Context& ctx) const
{
    auto mt = metric.type_;

    switch (mt) {
    case ai::MetricType::MeleeRange: {
            Q_ASSERT(ctx.actor_);
            auto ent = ctx.actor_->as_entity();
            if (not ent) { return ai::Metric(mt); }
            auto pos = ent->get_pos();
            auto in_melee = adjacent_or_same(pos, ctx.pos_);
            return metric.set_reading(in_melee);
        } break;

    case ai::MetricType::HaveHand: {
            Q_ASSERT(ctx.actor_);
            auto bo = ctx.actor_->as_body();
            if (not bo) { return ai::Metric(mt); }
            auto has_hand = bo->has_body_part(BodyComponent::PartType::Hand);
            return metric.set_reading(has_hand);
        } break;

    case ai::MetricType::HaveLeg: {
            Q_ASSERT(ctx.actor_);
            auto bo = ctx.actor_->as_body();
            if (not bo) { return ai::Metric(mt); }
            auto has_leg = bo->has_body_part(BodyComponent::PartType::Leg);
            return metric.set_reading(has_leg);
        } break;

    case ai::MetricType::HaveMiningPick: {
            return metric.set_reading(true);
        } break;

    case ai::MetricType::BlockIsNotSolid: {
            // air or liquid will satisfy the condition
            auto vox = get_voxel(metric.arg_.get_pos());
            return metric.set_reading(not bm::is_solid(vox));
        } break;
    }
}

ai::Order::Ptr World::get_random_desire(ComponentObject *actor,
                                        ai::OrderMap &registry)
{
    // Pick a random order. Check if it is not fulfilled yet. Give out.
    while (not registry.empty()) {
        std::uniform_int_distribution<size_t> rand_id(0, registry.size()-1);

        auto iter = registry.begin();
        std::advance(iter, rand_id(rand_));

        ai::Order::Ptr rnd_order = iter->second;
        ai::Context ctx(rnd_order->ctx_); // copy
        ctx.actor_ = actor;
        if (conditions_stand_true(rnd_order->desired_, ctx)) {
            // Desire is fulfilled, we do not share it with actors anymore
            qDebug() << "world: Order conditions stand true, deleting";
            registry.erase(iter);
            continue;
        }
        return rnd_order;
    }
    return nullptr;
}

void World::lower_prio(ai::OrderId id)
{
    auto low_iter = orders_low_.find(id);
    if (low_iter != orders_low_.end()) {
        orders_verylow_[id] = low_iter->second;
        orders_low_.erase(id);
        return;
    }
    auto iter = orders_.find(id);
    if (iter != orders_.end()) {
        orders_low_[id] = iter->second;
        orders_.erase(id);
    }
}

//void World::add_position_order(const Vec3i &pos, JobType jt) {
//    orders_.insert(std::make_shared<PositionOrder>(pos, jt));
//}

} // ns bm
