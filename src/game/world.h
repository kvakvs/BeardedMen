#pragma once

//#include <set>
#include <random>

#include "game/co_entity.h"
#include "game/order.h"

#include "ai/goal.h"

#include "world_pager.h"

namespace bm {

class World {
public:
    World(bm::RawVolume& vol): volume_(vol) {
    }
    void add(ComponentObject *ent);

    template <typename EachObjFn>
    void each_obj(EachObjFn fn) {
        for (auto pair: objects_) {
            fn(pair.first, pair.second);
        }
    }

    void think();

    VoxelType get_under(const Vec3i &pos) const {
        return volume_.getVoxel(pos.getX(), pos.getY()+1, pos.getZ());
    }

//    void add_position_order(const Vec3i &pos, JobType jt);
    bool is_mineable(const Vec3i &pos) const;
    bm::VolumeType* get_volume() { return &volume_; }
    void mine_voxel(const Vec3i &pos);
    VoxelType get_voxel(const Vec3i& pos) const {
        return volume_.getVoxel(pos);
    }

    //
    // Orders, planning and AI
    //

    // Check if any orders are available
    bool have_orders() const { return desired_changes_.empty() == false; }
    bool add_goal(const ai::MetricVec& desired);
    // Get a random order. See if it is not completed.
    ai::MetricVec get_random_desire();
    void add_mining_goal(const Vec3i& pos);

    // For every metric, read current situation and compare
    bool conditions_stand_true(const ai::MetricVec& cond,
                               const ComponentObject* subject) const;
    // For every condition type in desired, reads current situation
    ai::MetricVec get_current_situation(const ai::MetricVec& desired,
                                        const ComponentObject* co) const;
    ai::Metric read_metric(const ai::Metric& metric,
                           const ComponentObject* subject) const;

public:
    bool any_voxel_changed_ = false;

private:
    std::mt19937 rand_;

    uint64_t ent_id_ = 0;
    std::map<EntityId, ComponentObject *> objects_;
    // Visible piece of world + some nearby
    bm::VolumeType& volume_;

    // What player desires (goals of sort, without preconditions) - will
    // propagate to workers and they will see how to fulfill master's wish.
    std::vector<ai::MetricVec> desired_changes_;
};


inline uint64_t square_distance(const Vec3i& a, const Vec3i& b) {
    auto dx = b.getX() - a.getX();
    auto dy = b.getY() - a.getY();
    auto dz = b.getZ() - a.getZ();
    return dx*dx + dy*dy + dz*dz;
}

inline bool adjacent_or_same(const Vec3i& a, const Vec3i& b) {
    auto dx = b.getX() - a.getX();
    auto dy = b.getY() - a.getY();
    auto dz = b.getZ() - a.getZ();
    return (dy == 0) && (
                (std::abs(dx) <= 1 && dz == 0)
                || (std::abs(dz) <= 1 && dx == 0)
            );
}

} // ns bm
