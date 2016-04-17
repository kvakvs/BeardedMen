#pragma once

//#include <set>
#include <random>

#include "game/co_entity.h"
#include "game/order.h"
#include "ai/goal.h"
#include "util/optional.h"
#include "world_volume.h"

namespace bm {

using ObjectMap = std::map<EntityId, ComponentObject *>;

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
    const bm::VolumeType* get_volume() const { return &volume_; }

    void mine_voxel(const Vec3i &pos);
    VoxelType get_voxel(const Vec3i& pos) const {
        return volume_.getVoxel(pos);
    }

    //
    // Orders, planning and AI
    //

    // Check if any orders are available
    bool have_orders() const;
    bool add_order(ai::Order::Ptr desired);
    void remove_order(ai::OrderId id);
    // Get a random order. See if it is not completed.
    ai::Order::Ptr get_random_order(ComponentObject *actor);
    void add_mining_goal(const Vec3i& pos);
    // Report from actors if order is completed
    void report_fulfilled(ai::OrderId id);
    // Report from actors if order is failed
    void report_failed(ai::OrderId id);
    void report_impossible(ai::OrderId id);

    // Rendering helpers allow to view some data
    const ai::OrderMap& get_orders() const { return orders_; }
    const ai::OrderMap& get_orders_low() const { return orders_low_; }
    const ai::OrderMap& get_orders_verylow() const { return orders_verylow_; }
    const ObjectMap& get_objects() const { return objects_; }

    //
    // Sensors and metrics system
    //
    // For every metric, read current situation and compare
    bool conditions_stand_true(const ai::MetricVec& cond,
                               const ai::Context& ctx) const;
    // For every condition type in desired, reads current situation
    ai::MetricVec get_current_situation(const ai::MetricVec& desired,
                                        const ai::Context& ctx) const;
    ai::Metric read_metric(const ai::Metric& metric,
                           const ai::Context& ctx) const;

private:
    ai::Order::Ptr get_random_order(ComponentObject *actor,
                                     ai::OrderMap& registry);
    void lower_prio(ai::OrderId id);

public:
    bool force_update_terrain_mesh_ = false;

private:
    std::mt19937 rand_;

    uint64_t ent_id_ = 0;
    ObjectMap objects_;
    // Visible piece of world + some nearby
    bm::VolumeType& volume_;

    // TODO: Move desires that failed too many times to low priority list

    // What player desires (goals of sort, without preconditions) - will
    // propagate to workers and they will see how to fulfill master's wish.
    ai::OrderMap orders_;
    ai::OrderMap orders_low_; // lower prio
    ai::OrderMap orders_verylow_; // even lower prio
    constexpr static uint64_t LOW_PRIO_ORDERS_EVERY = 3;
    constexpr static uint64_t VERY_LOW_PRIO_ORDERS_EVERY = 6;

    // Simulation step (equivalent of time)
    uint64_t sim_step_ = 0;
};


inline int64_t square_distance(const Vec3i& a, const Vec3i& b) {
    int64_t dx = b.getX() - a.getX();
    int64_t dy = b.getY() - a.getY();
    int64_t dz = b.getZ() - a.getZ();
    return dx*dx + dy*dy + dz*dz;
}

inline bool adjacent_or_same(const Vec3i& a, const Vec3i& b) {
    int dx = b.getX() - a.getX();
    int dy = b.getY() - a.getY();
    int dz = b.getZ() - a.getZ();
    return (dy == 0) && (
                (std::abs(dx) <= 1 && dz == 0)
                || (std::abs(dz) <= 1 && dx == 0)
            );
}

} // ns bm
