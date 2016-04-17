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
    each_obj([this](auto /*id*/, auto co) {
        BrainsComponent* brains = co->as_brains();
        if (brains) {
            brains->think();

            if (not desires_.empty()) {
                auto dsr = get_random_desire(co);
                if (dsr.has_value()) {
                    // Now he wants to do the order, unless it's impossible
                    brains->want(dsr.get_value());
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

bool World::add_goal(const ai::MetricContextPair& desired) {
    desires_.push_back(desired);
    return true;
}

Optional<ai::MetricContextPair> World::get_random_desire(ComponentObject *actor) {
    // Pick a random order. Check if it is not fulfilled yet. Give out.
    while (not desires_.empty()) {
        std::uniform_int_distribution<size_t>
                rand_id(0, desires_.size()-1);
        size_t oid = rand_id(rand_);

        auto some_desire = desires_[oid];
        ai::Context ctx(some_desire.second);
        ctx.actor_ = actor;
        if (conditions_stand_true(some_desire.first, ctx)) {
            // Desire is fulfilled, we do not share it with actors anymore
            qDebug() << "world: Wish is true, deleting";
            desires_.erase(desires_.begin() + oid);
            return {};
        }

        //qDebug() << "world: get_random_desire" << some_desire.first;
        return some_desire;
    }
    return {};
}

void World::add_mining_goal(const Vec3i &pos)
{
    // TODO: Remove melee range requirement
    // This affects planning (i.e. planning is stupid enough to not find out
    // a requirement to move into melee range, or possibly a bug)
    ai::MetricVec m { ai::Metric(ai::MetricType::BlockIsNotSolid,
                                 ai::Value(pos),
                                 ai::Value(true)),
                      ai::Metric(ai::MetricType::MeleeRange,
                                 ai::Value(),
                                 ai::Value(true))
                    };
    ai::Context ctx(this, nullptr);
    ctx.pos_ = pos;

    auto mc_pair = std::make_pair(m, ctx);

    if (add_goal(mc_pair)) {
        qDebug() << "Player wishes to mine out a block" << pos;
    }
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
            //auto in_melee = adjacent_or_same(pos, metric.arg_.get_pos()) <= 1;
            auto in_melee = adjacent_or_same(pos, ctx.pos_);
            qDebug() << "melee_range check" << pos << ctx.pos_ << in_melee;
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

//void World::add_position_order(const Vec3i &pos, JobType jt) {
//    orders_.insert(std::make_shared<PositionOrder>(pos, jt));
//}

} // ns bm
