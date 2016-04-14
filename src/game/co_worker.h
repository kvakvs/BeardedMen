#pragma once

#include <stdint.h>

#include "game/order.h"

namespace bm {

// Is able to take orders
class WorkerComponent {
public:
    WorkerComponent() {}

    bool is_idle() const { return (bool)order_; }

    // Returns true if order accepted
    bool take_order(Order::Ptr order);

private:
    Order::Ptr order_;
};

} // ns bm
