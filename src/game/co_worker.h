#pragma once

#include <stdint.h>
#include <list>

#include "game/order.h"

namespace bm {

// Is able to take orders
class WorkerComponent {
public:
    WorkerComponent(): order_() {}

    bool is_idle() const { return ! ((bool)order_); }

    // Returns true if order accepted
    // arg Co is outer object that contains this worker component
    bool take_order(ComponentObject* co, Order::Ptr order);

    // We have an order. See that it is done
    void perform(World& wo, ComponentObject* co);

private:
    Order::Ptr order_;
    std::list<Vec3i> path_;

private:
    void perform_position_order(World& wo,
                                ComponentObject* co,
                                PositionOrder* po);
};

} // ns bm
