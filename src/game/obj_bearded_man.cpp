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
        using V    = ai::Value;

        bm_actions.push_back(ai::ActionDef( AT::Move,
            // Requires
            { Mtrc(CT::HaveLeg, V(), V(true) /* needs subject */)
            },
            // Gives
            { Mtrc(CT::MeleeRange, V(), V(true)) /* needs pos */
            }));

        bm_actions.push_back(ai::ActionDef( AT::MoveClose,
            // Requires
            { Mtrc(CT::HaveLeg, V(), V(true) /* needs subject */)
            },
            // Gives
            { Mtrc(CT::MeleeRangeDepth, V(), V(true)) /* needs pos */
            }));

        bm_actions.push_back(ai::ActionDef( AT::Mine,
            // Requires
            { Mtrc(CT::MeleeRangeDepth, V(), V(true)), // needs arg (pos)
              Mtrc(CT::HaveHand, V(), V(true)), /* needs subject */
              Mtrc(CT::HaveMiningPick, V(), V(true) /* needs subject */)
            },
            // Gives
            { Mtrc(CT::BlockIsNotSolid, V(), V(true) /* needs arg=pos */)
            }));

        bm_actions.push_back(ai::ActionDef( AT::CreateRamp,
            // Requires
            { Mtrc(CT::MeleeRangeDepth, V(), V(true)), // needs arg (pos)
              Mtrc(CT::HaveHand, V(), V(true)), /* needs subject */
              Mtrc(CT::HaveMiningPick, V(), V(true) /* needs subject */)
            },
            // Gives
            { Mtrc(CT::BlockIsRamp, V(), V(true) /* needs arg=pos */)
            }));

        is_initialized = true;
    }

    return bm_actions;
}

} // ns bm
