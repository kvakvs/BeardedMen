#pragma once

//#include <set>
#include <random>
#include <point_multimap.hpp>

#include "game/co_entity.h"
#include "game/order.h"
#include "ai/goal.h"
#include "util/optional.h"
#include "world_volume.h"
#include "game/g_defs.h"

namespace bm {

using ObjectMap = std::map<EntityId, AnimateObject *>;

using SpatialObjectMap = spatial::point_multimap<3, Vec3i, AnimateObject *>;

class InanimateObject {
public:
    using Ptr = std::shared_ptr<InanimateObject>;
    InanimateType type_;
    InanimateObject(InanimateType t): type_(t) {}
};

using SpatialInanimateMap = spatial::point_multimap<
                                3, Array3i, InanimateObject::Ptr>;

class World {
public:
    World(bm::RawVolume& vol): volume_(vol) {
    }
    void add_component_object(AnimateObject *ent);
    void spawn_inanimate_object(const Vec3i& pos, InanimateType ot);

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
    ai::Order::Ptr get_random_order(AnimateObject *actor);

    // Give positional or area mining goal
    void add_mining_goal(const Optional<Vec3i>& mark_begin,
                         const Vec3i& pos);
    void add_mining_goal(const Vec3i& pos);

    // Report from actors if order is completed
    void report_fulfilled(ai::OrderId id, PlanResult pr);
    // Report from actors if order is failed
    void report_failed(ai::OrderId id, PlanResult pr);
    void report_impossible(ai::OrderId id, PlanResult pr);

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
    ai::Order::Ptr get_random_order(AnimateObject *actor,
                                     ai::OrderMap& registry);
    void lower_prio(ai::OrderId id);

public:
    bool force_update_terrain_mesh_ = false;

private:
    std::mt19937 rand_;
    std::uniform_int_distribution<int> d100_ {0, 99};

    uint64_t ent_id_ = 0;
    ObjectMap objects_;
    //SpatialObjectMap spatial_objects_;
    SpatialInanimateMap inanimate_;

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

} // ns bm
