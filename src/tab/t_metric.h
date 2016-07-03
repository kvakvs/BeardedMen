#pragma once

#include <stdint.h>
//#include <QDebug>

namespace bm {
namespace ai {

// Desired effects
enum class MetricType: uint16_t {
    // Target cell is in hand's reach standing on same depth
    MeleeRange,
    // Target is in hand's reach and depth is approximate +1/-1
    MeleeRangeDepth,

    // Block was extracted using tools or blasted in some way. Position is in
    // the context, passed separately
    BlockIsNotSolid,
    BlockIsRamp,

    HaveLeg,        // Creature has at least one leg
    HaveHand,       // Creature has at least one hand
    HaveMiningPick, // Creature has a tool
};

//QDebug operator<< (QDebug d, MetricType mt);

} // ns bm::ai
} // ns bm
