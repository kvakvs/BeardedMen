#pragma once

#include <stdint.h>
#include <list>

#if 0

#include "game/order.h"

namespace bm {

// Is able to take orders
class WorkerComponent {
public:
    WorkerComponent() {}

    //bool is_idle() const { return ! ((bool)order_); }

    // Returns true if order accepted
    // arg Co is outer object that contains this worker component
    bool take_order(ComponentObject* co, Order::Ptr order);

    // We have an order. See that it is done
    void perform(World& wo, ComponentObject* co);

private:
    //Order::Ptr order_;

private:
    void perform_position_order(World& wo,
                                ComponentObject* co,
                                PositionOrder* po);
    void plan_path_to(World &wo, PositionOrder *po, EntityComponent *ent);
};

} // ns bm

#endif //0
