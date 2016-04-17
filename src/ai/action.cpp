#include "ai/action.h"

namespace bm {namespace ai {

void ActionDef::copy_readings(MetricVec &state) const {
    for (Metric& have: state) {
        for (const Metric & gives: gives_) {
            if (have.type_ == gives.type_) {
                have.reading_ = gives.reading_;
                break;
            }
        }
    }
}

QDebug operator<<(QDebug d, const ActionDef &ad)
{
    d.nospace() << "ADef{act=" << ad.action_ << ", req=" << ad.requires_;
    d << "}";
    return d;
}

QDebug operator<<(QDebug d, ActionType at)
{
    d.nospace();
    switch (at) {
    case ActionType::None: d << "None"; break;
    case ActionType::Mine: d << "Mine"; break;
    case ActionType::Move: d << "Move"; break;
    }
    return d;
}

float get_action_cost(ActionType at) {
    switch (at) {
    case ActionType::None: return 0.1f;
    case ActionType::Mine: return 50.0f;
    case ActionType::Move: return 10.0f;
    }
}

}} // ns bm::ai
