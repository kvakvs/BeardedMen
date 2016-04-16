#pragma once

#include <vector>
#include <stdint.h>

#include "game/order.h"

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
    // Maybe store just count of multiple parts and take them away one by one
    class Part {
    public:
        PartType    type_:      8;
        bool        is_vital_:  1;
        uint32_t    hit_points_:23;
    };

    /*bool can_perform_job(JobType) const {
        // Here check if worker has enough limbs. So far all types of work
        // require one hand
        for (Part part: parts_) {
            if (part.type_ == PartType::Hand) {
                return true;
            }
        }
        return false;
    }*/
    bool has_body_part(PartType t) const {
        for (Part part: parts_) {
            if (part.type_ == t) { return true; }
        }
        return false;
    }
private:
    std::vector<Part> parts_;
    int hunger_;        // fix with food
    int exhaustion_;    // fix with sleep
};

} // ns bm
