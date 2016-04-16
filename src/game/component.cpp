#include "game/component.h"

#include <qDebug>
#include <QSettings>

namespace bm {

ai::ActionVec ComponentObject::ai_choose_actions(const ai::Goal &g) const
{
    // From static list of actions available for this class
    // (TODO: boot action list from a script)
    // select several appropriate actions which make goal closer
    ai::ActionVec plan;

    auto& actions = ai_get_all_actions();

    return plan;
}

ai::ActionVec ComponentObject::ai_load_actions(const char* script)
{
    ai::ActionVec result;
#if 0
    static QSettings afile("data/actions.txt", QSettings::IniFormat);

    afile.beginReadArray(script);
    auto all_keys = afile.allKeys();
    result.reserve(all_keys.size());

    for(auto k: all_keys) {
        qDebug() << k << afile.value(k).toString();
        //result.push_back(ai::Action::from_conf_string(v));
    }
#endif//0
    return result;
}

} // ns bm
