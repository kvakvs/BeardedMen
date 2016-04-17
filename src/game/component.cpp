#include "game/component.h"

#include <QDebug>
#include <QSettings>
#include "game/co_entity.h"

namespace bm {

World *ComponentObject::world_ = nullptr;

QDebug operator<<(QDebug d, const ComponentObject& co)
{
    d.nospace() << "Obj{";
    auto ent = co.as_entity();
    if (ent) { d << *ent; }
    d << "}";
    return d;
}

} // ns bm
