#pragma once

#include "util/vec.h"
#include "game/world_volume.h"
#include "model_id.h"

namespace bm {
namespace visual {

class Ramp {
public:
    Vec3f   pos;
    ModelId model_id;
    float   rotation;

    // Returns false if ramp cannot be created (and should be removed)
    static bool add_ramp(const Vec3f& p, const VoxelReader& vr,
                         std::vector<Ramp>& container);
};

} // ns bm::visual
} // ns bm
