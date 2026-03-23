#pragma once
#include "game/object/Actor.h"

struct CoreDeps : ActorDeps {
    class RenderDB& render;
    class PhysWorld& phys;
    class ConfigDB& config;

    CoreDeps(class RenderDB& render, class PhysWorld& phys,
             class ConfigDB& config)
        : ActorDeps(), render(render), phys(phys), config(config) {}
};

class Core : public Actor {
    class ConfigDB& config;

   public:
    Core(class ActorsSystem*, CoreDeps&);
    void UpdateActor(float) override;
};
