#include "ai/goal.h"

#include <QDebug>

#include "game/world.h"
#include "game/component.h"

#include "world_pager.h"

namespace bm {namespace ai {

Vec3i Value::get_pos() const
{
    Q_ASSERT(is_position());
    return Vec3i(pos_.x, pos_.y, pos_.z);
}

//bool Goal::are_precond_fulfilled(const World& wo,
//                                 const ComponentObject* subject) const {
//    return wo.conditions_stand_true(precond_, subject);
//}

//bool Goal::is_fulfilled(const World &wo,
//                             const ComponentObject* subject) const {
//    return wo.conditions_stand_true(desired_, subject);
//}

}} // ns ai::bm
