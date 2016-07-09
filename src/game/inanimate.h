#pragma once

#include "point_multimap.hpp"

#include "game/g_defs.h"
#include "util/types.h"

namespace bm {

class InanimateObject {
public:
    //using Ptr = std::shared_ptr<InanimateObject>;

    InanimateType type_;
    ModelId       model_;

    InanimateObject(InanimateType t);
};

using SpatialInanimateMap = spatial::point_multimap<
                                3, Array3i, InanimateObject>;

} // ns bm
