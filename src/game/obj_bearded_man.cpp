#include "game/obj_bearded_man.h"

namespace bm {

const ai::ActionVec &BeardedMan::ai_get_all_actions() const
{
    static bool is_initialized = false;
    static ai::ActionVec bm_actions;

    if (not is_initialized) {
        //bm_actions = ai_load_actions("bearded_man");
        using AT   = ai::ActionType;
        using G    = ai::Goal;
        using Cond = ai::Condition;
        using CT   = ai::CondType;

        ai::Action a1(AT::Walk,                // id
                     G({ Cond(CT::HaveLeg) }, // requires
                       Cond(CT::NearPosition)) //gives
                     );
        bm_actions.push_back(a1);

        ai::Action a2(AT::Mine,                // id
                     G({ Cond(CT::HaveHand),
                         Cond(CT::HaveMiningPick) }, // req
                       Cond(CT::BlockMined)) //gives
                     );
        bm_actions.push_back(a2);

        is_initialized = true;
    }

    return bm_actions;
}

} // ns bm
