#include "tab/t_metric.h"

namespace bm {
namespace ai {

QDebug operator<<(QDebug d, MetricType mt) {
    d.nospace();
    switch (mt) {
    case MetricType::MeleeRange: d << "MeleeRange"; break;
    case MetricType::MeleeRangeDepth: d << "MeleeRangeDepth"; break;
    case MetricType::BlockIsNotSolid: d << "BlockIsNotSolid"; break;
    case MetricType::HaveLeg: d << "HaveLeg"; break;
    case MetricType::HaveHand: d << "HaveHand"; break;
    case MetricType::HaveMiningPick: d << "HaveMiningPick"; break;
    }
    return d;
}

} // ns bm::ai
} // ns bm
