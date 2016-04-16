#pragma once

#include <stdint.h>
#include <vector>

#include "ai/goal.h"
#include "ai/action.h"

namespace bm {

//enum class ComponentId: uint32_t {
//    Entity,         // Has world position
//    Intelligent,    // can think()
//};

class EntityComponent;
class BrainsComponent;
class BodyComponent;
class WorkerComponent;

#define AS_COMPONENT(Lowercase, Capitalized) \
    virtual Capitalized##Component* as_##Lowercase() { \
        return nullptr;  \
    }

// Inherit this
class ComponentObject {
public:
    AS_COMPONENT(entity, Entity)
    AS_COMPONENT(brains, Brains)
    AS_COMPONENT(body,   Body)
    AS_COMPONENT(worker, Worker)

    // Filter list of available actions which make goal closer
    ai::ActionVec ai_choose_actions(const ai::Goal &g) const;
    // Initializes static vector of actions and returns constref
    virtual const ai::ActionVec& ai_get_all_actions() const = 0;
    static ai::ActionVec ai_load_actions(const char* script);
};


} // ns bm
