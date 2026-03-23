#pragma once
#include "HandsBase.h"

class RightHandCore : public HandsBase {
   public:
    RightHandCore(class ActorsSystem* actorSystem, class HandsBaseDeps& deps)
        : HandsBase(actorSystem, deps, 200, 10,
                    Vector3(255.0f / 256, 255.0f / 256, 162.0f / 256),
                    "handcore") {}
    void ActorInput(const pose::InputState& state) {
        HandsBase::ActorInput(state, pose::InputState::KEYPOINT_RIGHT_WRIST);
    }
};
