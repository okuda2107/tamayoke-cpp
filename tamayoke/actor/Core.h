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
    class RenderDB& render;
    class PhysWorld& phys;
    class ConfigDB& config;

    float mSpeed;
    size_t mCircleID;

    class SphereComponent* mSphereComp;

   public:
    bool isFlag;

    Core(class ActorsSystem*, CoreDeps&);
    void UpdateActor(float) override;
};
