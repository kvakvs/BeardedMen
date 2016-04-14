#pragma once

#include "game/component.h"
#include "game/co_body.h"

namespace bm {

// TODO: evict from this file, make own file
class BeardedMan: public ComponentObject
{
    EntityComponent entity_;
    BrainsComponent brains_;
    BodyComponent   body_;
public:
    BeardedMan(const Vec3i &pos)
        : entity_(pos, ModelId::BeardedMan),
          body_(BodyType::BeardedMan) {
    }
    virtual EntityComponent* as_entity() override { return &entity_; }
    virtual BrainsComponent* as_brains() override { return &brains_; }
};

} // ns bm
