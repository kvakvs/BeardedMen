#pragma once

#include <stdint.h>

namespace bm {

enum class ModelId: uint32_t {
    NIL = 0,
    Cursor, CursorRed,
    Wood,
    BeardedMan,
    Xyz,            // debug xyz axis
    MarkPick,
    MarkRoute,      // debug
};

} // ns bm
