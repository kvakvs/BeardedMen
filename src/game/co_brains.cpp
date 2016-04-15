#include "game/entity.h"
#include "game/world.h"

namespace bm {

void EntityComponent::step(const World &wo) {
    if (planned_route_.empty() == false) {
        if (attempt_move(wo, planned_route_.front())) {
            planned_route_.pop_front();
        } else {
            qDebug() << "Can't move along the planned route";
            planned_route_.clear();
        }
    }
}

bool EntityComponent::attempt_move(const World &wo, const Vec3i &new_pos)
{
    if (!is_solid(wo.get_voxel(new_pos))) {
        set_pos(new_pos);
        return true;
    }
    return false;
}


} // ns bm
