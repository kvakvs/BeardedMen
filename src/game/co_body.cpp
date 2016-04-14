#include "game/co_body.h"
#include <QDebug>

namespace bm {

BodyComponent::BodyComponent(BodyType bt)
{
    switch (bt) {
    case BodyType::BeardedMan:
        // Put hands first to speed up body-job-check which looks for a hand
        parts_.push_back(Part {PartType::Hand, false, 15});
        parts_.push_back(Part {PartType::Hand, false, 15});
        // Damaging head or torso will kill the man. Hands and legs are optional
        parts_.push_back(Part {PartType::Head, true, 20});
        parts_.push_back(Part {PartType::Torso, true, 30});
        parts_.push_back(Part {PartType::Leg,  false, 20});
        parts_.push_back(Part {PartType::Leg,  false, 20});
        parts_.push_back(Part {PartType::Foot, false, 20});
        parts_.push_back(Part {PartType::Foot, false, 20});
        return;
    }
    Q_ASSERT(false);
}

} // ns bm
