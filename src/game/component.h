#pragma once

#include <stdint.h>

namespace bm {

enum class ComponentId: uint32_t {
    Entity,         // Has world position
    Intelligent,    // can think()
};

class EntityComponent;
class BrainsComponent;

// Inherit this
class ComponentObject {
public:
    virtual EntityComponent* as_entity() {
        Q_ASSERT(false);
        return nullptr;
    }
    virtual BrainsComponent* as_brains() {
        Q_ASSERT(false);
        return nullptr;
    }
};


} // ns bm
