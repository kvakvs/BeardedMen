#pragma once

#include <vector>
#include <stdint.h>

namespace bm {

enum class BodyType: uint32_t {
    BeardedMan
};

class BodyComponent {
public:
    BodyComponent(BodyType bt);

    enum class PartType: uint8_t {
        Head, Hand, Leg, Foot, Torso
    };

    // TODO: slash damage to cut limbs
    class Part {
    public:
        PartType    type_:      8;
        bool        is_vital_:  1;
        uint32_t    hit_points_:23;
    };
private:
    std::vector<Part> parts_;
    int hunger_;        // fix with food
    int exhaustion_;    // fix with sleep
};

} // ns bm
