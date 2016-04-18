#pragma once

#include <stdint.h>
#include <QDebug>

namespace bm {
namespace ai {

// Desired effects
enum class MetricType: uint16_t {
    // Creature is in hand's reach. Position is in the context, passed
    // separately
    MeleeRange,
    // Block was extracted using tools or blasted in some way. Position is in
    // the context, passed separately
    BlockIsNotSolid,
    HaveLeg,        // Creature has at least one leg
    HaveHand,       // Creature has at least one hand
    HaveMiningPick, // Creature has a tool
};

QDebug operator<< (QDebug d, MetricType mt);

} // ns bm::ai
} // ns bm
