#pragma once

#include <stdint.h>

#include "vector.h"
#include "gfx/model.h"
#include "model_id.h"

namespace bm {

class World;
using EntityId = uint64_t;

// Entity is anything which has a position in the world.
// This includes both animate and inanimate objects (dropped resources,
// dead bodies etc)
class IEntity {
public:
    // Each entity has a position in 3d world (cell)
    virtual Vec3i get_pos() const = 0;
    virtual void set_pos(const Vec3i &v) = 0;
    // Each entity has ID so that we don't have to use pointers everywhere
    virtual EntityId get_id() const = 0;
    virtual void set_id(EntityId id) = 0;
};

class IHasModel {
public:
    // Draw me like one of your cubic models
    virtual ModelId get_model_id() = 0;
};

class Entity: public IEntity {
    Vec3i pos_;
    EntityId id_;
public:
    // -- Entity --
    virtual Vec3i get_pos() const override { return pos_; }
    virtual void set_pos(const Vec3i &v) override { pos_ = v; }
    virtual EntityId get_id() const override { return id_; }
    virtual void set_id(EntityId id) override { id_ = id; }
};

class IIntelligent {
public:
    virtual void think(const World &w) = 0;
};

class BeardedMan:
        public IIntelligent,
        public IHasModel,
        public Entity
{
public:
    BeardedMan(const Vec3i &pos) {
        this->set_pos(pos);
    }

    // -- Intelligent --
    virtual void think(const World &w) override;
    // -- Renderable --
    virtual ModelId get_model_id() override {
        return ModelId::BeardedMan;
    }
};

} // namespace bm
