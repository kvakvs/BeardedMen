#pragma once

#include <stdint.h>

namespace bm {

//enum class ComponentId: uint32_t {
//    Entity,         // Has world position
//    Intelligent,    // can think()
//};

class EntityComponent;
class BrainsComponent;
class BodyComponent;

#define AS_COMPONENT(Lowercase, Capitalized) \
    virtual Capitalized##Component* as_##Lowercase() { \
        Q_ASSERT(false); \
        return nullptr;  \
    }

// Inherit this
class ComponentObject {
public:
    AS_COMPONENT(entity, Entity)
    AS_COMPONENT(brains, Brains)
    AS_COMPONENT(body,   Body)
};


} // ns bm
