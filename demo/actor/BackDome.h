#pragma once
#include "game/object/Actor.h"

struct BackDomeDeps {
    class RenderDB& renderDB;

    BackDomeDeps(class RenderDB& render) : renderDB(render) {}
};

class BackDome : public Actor {
    float mTime;
    class SkydomeComponent* sc;

   public:
    BackDome(class ActorsSystem* system, BackDomeDeps deps);

    void UpdateActor(float deltatime) override;
};
