#pragma once

#include <stdint.h>

namespace bm {

// TODO: Remember to stop after uint8 is full! And extend data types!
// TODO: Rewrite class VoxelType and make a proper 0=air and everything else
enum class BlockType: uint16_t {
                // DISCLAIMER: all properties below are TODO
                //------------------------------------------
    AIR = 0,    // now with density actually value is not used, any cell can
                // have 0 density
    //--- Base natural materials ---
    Mud = 1,    // stuff grows on mud, even in cave, easy to dig
    Clay,       // easy to dig, does not grow stuff, produces pottery
    Sand,       // dry, does not grow stuff, can slide, producess glass
    Rock,       // hard to dig, produces stone

    Water,      // flows, floods if released, can move water to wetten mud
    Lava,
};


} // ns bm
