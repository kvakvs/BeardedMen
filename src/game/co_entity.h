#pragma once

#include <stdint.h>

#include "util/vec.h"
#include "gfx/model.h"
#include "model_id.h"
#include "game/component.h"

namespace bm {

class World;
using EntityId = uint64_t;
using Route = std::list<Vec3i>;

// Entity is anything which has a position in the world and is visible in
// some way (has model or a sprite etc).
// This includes both animate and inanimate objects (dropped resources,
// dead bodies etc)
class EntityComponent {
    ComponentObject *parent_;
    Vec3i pos_;
    EntityId id_;
    ModelId model_id_;

    struct {
        //bool is_moving_ = false;
        Route planned_route_;
        Vec3i dst_;
    } movement;

public:
    EntityComponent(ComponentObject *p, const Vec3i &pos, ModelId mod)
        : parent_(p), pos_(pos), model_id_(mod) {
    }

    // Each entity has a position in 3d world (cell)
    virtual Vec3i get_pos() const { return pos_; }
    virtual void set_pos(const Vec3i &v) { pos_ = v; }

    // Each entity has ID so that we don't have to use pointers everywhere
    // -- Entity --
    virtual EntityId get_id() const { return id_; }
    virtual void set_id(EntityId id) { id_ = id; }

    // Draw me like one of your cubic models
    virtual ModelId get_model_id() { return model_id_; }
    ComponentObject* get_parent() { return parent_; }

    // If route is planned, do a step. TODO: Respect movement speed
    void step();
    bool attempt_move(const Vec3i &new_pos);

    // Routing
    void move_to(const Vec3i& pos);
    void clear_planned_route() { movement.planned_route_.clear(); }
    bool is_moving() const {
        return movement.planned_route_.empty() == false;
    }
    void set_planned_route(const Vec3i& dst, Route &r) {
        movement.dst_ = dst;
        movement.planned_route_ = std::move(r);
    }
    Vec3i get_move_destination() const { return movement.dst_; }
};

class Entity: public ComponentObject {
    EntityComponent entity_;
public:
    EntityComponent* as_entity() override { return &entity_; }
};

} // namespace bm
