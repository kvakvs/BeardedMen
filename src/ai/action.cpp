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

void flesh_out_a_plan(Activities& out_plan,
                      const ActionVec &actions,
                      const Context &ctx)
{
    out_plan.clear();
    for (auto a: actions) {
        out_plan.push_back(Activity { a, Value(ctx.pos_) });
    }
}

QDebug operator<<(QDebug d, ActionVec av)
{
    d.nospace() << "Actions[";
    for (auto at: av) {
        d << at << "; ";
    }
    d << "]";
    return d;
}

}} // ns bm::ai
