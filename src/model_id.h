#pragma once

#include <stdint.h>

namespace bm {

enum class ModelId: uint32_t {
    NIL = 0,
    Cursor, CursorRed,

    Xyz,            // debug xyz axis
    MarkPick,       // mark areas designated for digging
    MarkRoute,      // debug: red route pin
    MarkArea,       // green X to mark area for commands
    MarkRamp,

    BeardedMan,

    Ramp_B,         // Ramps are based on ABCD notation
    Ramp_AB,        //   A
    Ramp_AC,        // D x B
    Ramp_ABC,       //   C

    Wood,
    Boulder,
};

constexpr int NUM_RAMPS = 4;

} // ns bm
