#include "tab/t_action.h"

namespace bm {
namespace ai {

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

} // ns bm::ai
} // ns bm
