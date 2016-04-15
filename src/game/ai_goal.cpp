#include "ai_goal.h"

#include <QDebug>

#include "game/world.h"
#include "world_pager.h"

namespace bm {namespace ai {

bool Condition::is_fulfilled_glob(const World &wo) const {
    switch (eff_) {
    case CondType::NearPosition:
        // Check for near_position requires entity, so global check always fails
        return false;
    case CondType::BlockMined:
        // air or liquid will satisfy the condition
        return false == bm::is_solid(wo.get_voxel(arg_.get_pos()));
    }
}

Vec3i Value::get_pos() const
{
    Q_ASSERT(is_position());
    return Vec3i(pos_.x, pos_.y, pos_.z);
}


}} // ns ai::bm
