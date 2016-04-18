#pragma once

#include <stdint.h>
#include <vector>

#include "ai/goal.h"
#include "ai/action.h"

namespace bm {

class EntityComponent;
class BrainsComponent;
class BodyComponent;
class WorkerComponent;

#define AS_COMPONENT(Lowercase, Capitalized) \
    virtual Capitalized##Component* as_##Lowercase() { \
        return nullptr;  \
    } \
    virtual const Capitalized##Component* as_##Lowercase() const { \
        return nullptr;  \
    }

// Inherit this
class AnimateObject {
public:
    //using Ptr = std::shared_ptr<AnimateObject>;

    AS_COMPONENT(entity, Entity)
    AS_COMPONENT(brains, Brains)
    AS_COMPONENT(body,   Body)
    AS_COMPONENT(worker, Worker)

    AnimateObject(World *wo) {
        // No other world can get here
        Q_ASSERT(world_ == nullptr || world_ == wo);
        world_ = wo;
    }
    static World* get_world() { return world_; }

    // Override this for descendants to change available actions (or script it)
    virtual const ai::ActionDefVec& ai_get_all_actions() const = 0;

private:
    // static?
    static World *world_;
};

QDebug operator<< (QDebug d, const AnimateObject& co);

} // ns bm
