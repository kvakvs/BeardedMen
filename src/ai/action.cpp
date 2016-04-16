#include "ai/action.h"

namespace bm {namespace ai {

void ActionDef::apply_to(MetricVec &state) const {
    for (Metric& have: state) {
        for (const Metric & gives: gives_) {
            if (have.type_ == gives.type_) {
                have.arg_ = gives.arg_;
                break;
            }
        }
    }
}

}} // ns bm::ai
