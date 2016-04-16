#include "game/obj_bearded_man.h"

namespace bm {

const ai::ActionDefVec &BeardedMan::ai_get_all_actions() const
{
    static bool is_initialized = false;
    static ai::ActionDefVec bm_actions;

    if (not is_initialized) {
        //bm_actions = ai_load_actions("bearded_man");
        using AT   = ai::ActionType;
        using Mtrc = ai::Metric;
        using CT   = ai::MetricType;

        bm_actions.push_back(ai::ActionDef(
            AT::Walk, { Mtrc(CT::HaveLeg) }, { Mtrc(CT::NearPosition) }));
        bm_actions.push_back(ai::ActionDef(
            AT::Mine, { Mtrc(CT::HaveHand), Mtrc(CT::HaveMiningPick) },
            { Mtrc(CT::BlockIsNotSolid) }));

        is_initialized = true;
    }

    return bm_actions;
}

} // ns bm
