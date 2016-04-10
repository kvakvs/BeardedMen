#pragma once

#include <memory>

#include "game/entity.h"
#include "vector.h"

namespace bm {

// Abstract order interface, that actors will adhere to.
// Order can be an area order (i.e. dig these halls) a position order (i.e.
// dig here, move here), target order (kill target).
enum class OrderType: uint32_t {
    Area,
    Position,
    Target
};

class IOrder {
public:
    using Ptr = std::shared_ptr<IOrder>;
    virtual OrderType get_type() const = 0;
};

class AreaOrder {
private:
    std::vector<Region> areas_;
public:
    virtual OrderType get_type() const override { return OrderType::Area; }
};

class PositionOrder {
private:
    Vec3i pos_;
public:
    virtual OrderType get_type() const override { return OrderType::Position; }
};

class TargetOrder {
private:
    std::vector<EntityId> targets_;
public:
    virtual OrderType get_type() const override { return OrderType::Target; }
};

} // namespace bm
