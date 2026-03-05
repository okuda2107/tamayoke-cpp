#pragma once

#include "game/object/Actor.h"

struct TimerDeps {};

class Timer : public Actor {
   public:
    float mTime;

    Timer(class ActorsSystem* system, TimerDeps deps)
        : Actor(system, ActorDeps{}), mTime(0.0f) {}

    void UpdateActor(float deltatime) override { mTime += deltatime; }
};
