#pragma once

#include <QDebug>
#include <stdint.h>
#include "util/vec.h"

namespace bm {

enum class MovePrecision { Exact, Adjacent, AdjacentDepth };

inline int64_t square_distance(const Vec3i& a, const Vec3i& b) {
    int64_t dx = b.getX() - a.getX();
    int64_t dy = b.getY() - a.getY();
    int64_t dz = b.getZ() - a.getZ();
    return dx*dx + dy*dy + dz*dz;
}

inline bool adjacent_or_same(const Vec3i& a, const Vec3i& b) {
    int dx = b.getX() - a.getX();
    int dy = b.getY() - a.getY();
    int dz = b.getZ() - a.getZ();
    return (dy == 0) && (
                (std::abs(dx) <= 1 && dz == 0)
                || (std::abs(dz) <= 1 && dx == 0)
            );
}

inline bool adjacent_or_below_or_above(const Vec3i& a, const Vec3i& b) {
    int dx = b.getX() - a.getX();
    int dy = b.getY() - a.getY();
    int dz = b.getZ() - a.getZ();
    return std::abs(dy) <= 1
            && (
                (std::abs(dx) <= 1 && dz == 0)
                || (std::abs(dz) <= 1 && dx == 0)
            );
}

inline bool close_enough(const Vec3i& a, const Vec3i& b, MovePrecision mp) {
    switch (mp) {
    case MovePrecision::Adjacent: return adjacent_or_same(a, b);
    case MovePrecision::AdjacentDepth: return adjacent_or_below_or_above(a, b);
    case MovePrecision::Exact: return a == b;
    }
}


enum class PlanResult {
    Success,
    MoveStuck,
    MoveNoRoute,
    Done,           // does not mean succeeded, just done steps
    NoPlan,         // no sequence of actions found
};
QDebug operator<<(QDebug d, PlanResult pr); // defined in world.cpp

enum class InanimateType {
    Boulder
};

} // ns bm
