#pragma once
#include "game/object/Actor.h"

struct LogoDeps : ActorDeps {};

class Logo : public Actor {
   public:
    Logo(class ActorsSystem* system, LogoDeps& _);
};
