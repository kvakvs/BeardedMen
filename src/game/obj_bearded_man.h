#pragma once

#include "model_id.h"

#include "game/component.h"
#include "game/co_body.h"
#include "game/co_worker.h"
#include "game/co_brains.h"
#include "ai/action.h"

namespace bm {

// TODO: evict from this file, make own file
class BeardedMan: public ComponentObject
{
public:
    BeardedMan(const Vec3i &pos)
        : entity_(pos, ModelId::BeardedMan),
          body_(BodyType::BeardedMan) //, worker_()
    {
    }
    virtual EntityComponent* as_entity() override { return &entity_; }
    virtual BrainsComponent* as_brains() override { return &brains_; }
    virtual BodyComponent*   as_body()   override { return &body_;   }
    //virtual WorkerComponent* as_worker() override { return &worker_; }

    // Filter list of available actions which satisfy one of conditions
    //virtual ai::ActionVec ai_choose_actions(const ai::Goal& g) const override;

    // Initializes static vector of actions and returns constref
    virtual const std::vector<ai::Action>& ai_get_all_actions() const override;

private:
    EntityComponent entity_;
    BrainsComponent brains_;
    BodyComponent   body_;
    //WorkerComponent worker_;
};

} // ns bm
