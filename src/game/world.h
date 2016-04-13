#pragma once

#include "game/entity.h"
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

private:
    uint64_t ent_id_ = 0;
    std::map<EntityId, ComponentObject *> objects_;
    // Visible piece of world + some nearby
    bm::RawVolume& volume_;
};

} // ns bm
