#pragma once

#include <set>

#include "game/entity.h"
#include "game/order.h"

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

    void add_position_order(const Vec3i &pos, JobType jt);
    bool is_mineable(const Vec3i &pos) const;

private:
    uint64_t ent_id_ = 0;
    std::map<EntityId, ComponentObject *> objects_;
    // Visible piece of world + some nearby
    bm::RawVolume& volume_;
    // Orders!
    std::set<Order::Ptr> orders_;
};

} // ns bm
