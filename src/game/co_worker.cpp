#include "game/co_worker.h"
#include <QDebug>

namespace bm {

bool WorkerComponent::take_order(Order::Ptr order)
{
    order_ = order;
    return true;
}


} // ns bm
