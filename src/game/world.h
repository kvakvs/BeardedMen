#pragma once

#include <set>

#include "game/co_entity.h"
#include "game/order.h"
#include "game/ai_goal.h"

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

    // Check if any orders are available
    bool have_orders() const { return orders_.empty() == false; }
    bool add_order(const ai::Condition &order) {
        if (order.is_fulfilled_glob(*this) == false) {
            orders_.push_back(order);
            return true;
        }
        return false;
    }

public:
    bool any_voxel_changed_ = false;
private:
    uint64_t ent_id_ = 0;
    std::map<EntityId, ComponentObject *> objects_;
    // Visible piece of world + some nearby
    bm::VolumeType& volume_;

    // What player desires - will propagate to workers and they will see how
    // to fulfill master's wish
    std::vector<ai::Condition> orders_;
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
