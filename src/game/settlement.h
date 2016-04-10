#pragma once

#include <list>
#include <map>
#include <string>
#include <vector>

#include "game/entity.h"
#include "game/order.h"

namespace bm {

// My Awesome Spatially Optimized Data Structure for Entities!
template <typename ChunkId, typename T>
using SpatialStore = std::list<T>;

class Settlement {
    std::list<EntityId> settlers_;

    // Places where bearded men hang out. Random position from a random region
    // is taken when bearded man is bored and goes to rest
    //std::vector<Region>   dining_rooms_;

    // TODO: Garbage is also marked as storage room
    //std::vector<Region>   storage_rooms; // TODO: storage per type of goods

    // Current orders from the player. Some settlers may take them now or later
    std::list<IOrder::Ptr> orders_;
public:
};

} // namespace bm
