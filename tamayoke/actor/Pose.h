#pragma once
#include "game/object/IActor.h"
#include "input/pose/InputState.h"

struct PoseDeps : ActorDeps {};

class Pose : public IActor<pose::InputState> {
   public:
    Pose(class ActorsSystem* system, PoseDeps&);

    void ProcessInput(const pose::InputState& state);
};
