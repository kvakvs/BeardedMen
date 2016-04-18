#pragma once

#include <QDebug>
#include <algorithm>

#include <PolyVox/Vector.h>
#include <PolyVox/Region.h>
namespace pv = PolyVox;

namespace bm {

using Vec3u = pv::Vector3DUint32;
using Vec3i = pv::Vector3DInt32;
using Vec3d = pv::Vector3DDouble;
using Vec3f = pv::Vector3DFloat;

QDebug operator<< (QDebug d, const Vec3i &v);

using Region = pv::Region;

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

} // namespace bm
