#include "gfx/ramp.h"
#include "util/error.h"

namespace bm {
namespace visual {

bool Ramp::add_ramp(const Vec3f &p, const VoxelReader &vr,
                    std::vector<Ramp>& container)
{
    auto v  = vr.read(0, 0, 0);
    BM_ASSERT(not is_air(v));

    // surrounding voxels in clockwise order
    //   A
    // D x B
    //   C

    auto vA = vr.read(0, 0, -1);
    auto vB = vr.read(1, 0, 0);
    auto vC = vr.read(0, 0, 1);
    auto vD = vr.read(-1, 0, 0);

    auto airA = is_air(vA);
    auto airB = is_air(vB);
    auto airC = is_air(vC);
    auto airD = is_air(vD);

    Ramp r {p, ModelId::Ramp_B, 0.f};

    //   A
    // D x B
    //   C
    if (airA) {
        if (airB) {
            if (airC) {
                if (airD) {
                    // abcd - has no walls supporting it = collapsed
                    return false;
                } else {
                    // abcD
                    r.model_id = ModelId::Ramp_B;
                    r.rotation = 180.f;
                }
            } else {
                if (airD) {
                    // abCd
                    r.model_id = ModelId::Ramp_B;
                    r.rotation = -90.f;
                } else {
                    // abCD
                    r.model_id = ModelId::Ramp_AB;
                    r.rotation = 180.f;
                }
            }
        } else {
            if (airC) {
                if (airD) {
                    // aBcd
                    r.model_id = ModelId::Ramp_B;
                } else {
                    // aBcD
                    r.model_id = ModelId::Ramp_AC;
                    r.rotation = -90.f;
                }
            } else {
                if (airD) {
                    // aBCd
                    r.model_id = ModelId::Ramp_AB;
                    r.rotation = -90.f;
                } else {
                    // aBCD
                    r.model_id = ModelId::Ramp_ABC;
                    r.rotation = -90.f;
                }
            }
        }
    } else {
        if (airB) {
            if (airC) {
                if (airD) {
                    // Abcd
                    r.model_id = ModelId::Ramp_B;
                    r.rotation = +90.f;
                } else {
                    // AbcD
                    r.model_id = ModelId::Ramp_AB;
                    r.rotation = +90.f;
                }
            } else {
                if (airD) {
                    // AbCd
                    r.model_id = ModelId::Ramp_AC;
                } else {
                    // AbCD
                    r.model_id = ModelId::Ramp_ABC;
                    r.rotation = 180.f;
                }
            }
        } else {
            if (airC) {
                if (airD) {
                    // ABcd
                    r.model_id = ModelId::Ramp_AB;
                } else {
                    // ABcD
                    r.model_id = ModelId::Ramp_ABC;
                    r.rotation = +90.f;
                }
            } else {
                if (airD) {
                    // ABCd
                    r.model_id = ModelId::Ramp_ABC;
                } else {
                    // ABCD - surrounded by walls = collapsed
                    return false;
                }
            }
        }
    }

    container.push_back(r);
    return true;
}

} // ns bm::visual
} // ns bm
