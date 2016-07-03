#pragma once

//#include <QDebug>
#include <algorithm>

#include <PolyVox/Vector.h>
#include <PolyVox/Region.h>
namespace pv = PolyVox;

namespace bm {

using Vec3u = pv::Vector3DUint32;
using Vec3i = pv::Vector3DInt32;
using Vec3d = pv::Vector3DDouble;
using Vec3f = pv::Vector3DFloat;

inline bool operator< (const Vec3i& a, const Vec3i& b) {
    return a.getX() < b.getX()
            || (a.getX() == b.getX()
                && (a.getY() < b.getY()
                 || (a.getY() == b.getY()
                    && (a.getZ() < b.getZ()
                        || a.getZ() == b.getZ()
                        ))));
}

//QDebug operator<< (QDebug d, const Vec3i &v);

using Region = pv::Region;
using Array3i = std::array<int, 3>;

namespace util {

template <typename Vec>
Region make_region(const Vec &a, const Vec &b)
{
    return Region(std::min(a.getX(), b.getX()),
                  std::min(a.getY(), b.getY()),
                  std::min(a.getZ(), b.getZ()),
                  std::max(a.getX(), b.getX()),
                  std::max(a.getY(), b.getY()),
                  std::max(a.getZ(), b.getZ()));
}

inline Array3i make_array(const Vec3i& v) {
    return Array3i {v.getX(), v.getY(), v.getZ()};
}
inline Vec3i make_vec3i(const Array3i& a) {
    return Vec3i(a[0], a[1], a[2]);
}

// Runs function for each cell in region
template <typename EachCell>
void each_in_region(const pv::Region& reg, EachCell fn) {
    for (int y = reg.getLowerY(); y <= reg.getUpperY(); y++) {
        for (int x = reg.getLowerX(); x <= reg.getUpperX(); x++) {
            for (int z = reg.getLowerZ(); z <= reg.getUpperZ(); z++) {
                fn(x, y, z);
            }
        }
    }
}

} // ns util
} // ns bm
