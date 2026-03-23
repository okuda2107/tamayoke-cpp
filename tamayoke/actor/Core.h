#pragma once
#include "game/object/Actor.h"

struct CoreDeps : ActorDeps {
    class ConfigDB& config;

    CoreDeps(class ConfigDB& config) : ActorDeps(), config(config) {}
};

class Core : public Actor {
    class ConfigDB& config;

   public:
    Core(class ActorsSystem*, CoreDeps&);
    void UpdateActor(float) override;
};
