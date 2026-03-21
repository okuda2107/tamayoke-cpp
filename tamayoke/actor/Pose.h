#pragma once
#include "game/object/IActor.h"
#include "input/pose/InputState.h"

struct PoseDeps : ActorDeps {
    class RenderDB& renderDB;

    PoseDeps(class RenderDB& db) : ActorDeps(), renderDB(db) {}
};

class Pose : public IActor<pose::InputState> {
    class RenderDB& db;

   public:
    Pose(class ActorsSystem* system, PoseDeps& deps);

    void ActorInput(const pose::InputState& state);
};
