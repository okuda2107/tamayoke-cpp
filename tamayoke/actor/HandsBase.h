#pragma once
#include "game/object/IActor.h"
#include "input/pose/InputState.h"

struct HandsBaseDeps : ActorDeps {
    class RenderDB& renderDB;
    class PhysWorld& phys;
    class ConfigDB& configDB;

    HandsBaseDeps(class RenderDB& db, class PhysWorld& phys,
                  class ConfigDB& config)
        : ActorDeps(), renderDB(db), phys(phys), configDB(config) {}
};

class HandsBase : public IActor<pose::InputState> {
    class RenderDB& db;
    class ConfigDB& config;
    size_t mID;

   public:
    HandsBase(class ActorsSystem*, HandsBaseDeps&, size_t);
    void ActorInput(const pose::InputState& state, int);
};
