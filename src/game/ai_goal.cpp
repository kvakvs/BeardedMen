#include "ai_goal.h"

#include <QDebug>

#include "game/world.h"
#include "world_pager.h"

namespace bm {namespace ai {

Tribool Condition::is_fulfilled_glob(const World &wo) const {
    switch (cond_) {
    case CondType::NearPosition:
        // Check for near_position requires entity
        return Tribool::N_A;
    case CondType::BlockMined:
        // air or liquid will satisfy the condition
        return Tribool(
                    not bm::is_solid(wo.get_voxel(arg_.get_pos()))
                );
    case CondType::AlwaysTrue:
        return Tribool(true);
    }
}

Vec3i Value::get_pos() const
{
    Q_ASSERT(is_position());
    return Vec3i(pos_.x, pos_.y, pos_.z);
}

bool Goal::has_preconditions_glob(const World &wo) const {
    for (auto &pre: precond_) {
        if (pre.is_fulfilled_glob(wo).is_false()) {
            return false;
        }
    }
    return true;
}


}} // ns ai::bm
