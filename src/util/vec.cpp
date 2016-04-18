#include "util/vec.h"

namespace bm {

QDebug operator<<(QDebug d, const Vec3i &v)
{
    d.nospace() << "Vec3i(" << v.getX() << "," << v.getY() << ","
                << v.getZ() << ")";
    return d;
}

} // namespace bm
