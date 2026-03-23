#pragma once
#include "HandsBase.h"

class LeftHandCore : public HandsBase {
   public:
    LeftHandCore(class ActorsSystem* actorSystem, class HandsBaseDeps& deps)
        : HandsBase(actorSystem, deps, 201, 10,
                    Vector3(255.0f / 256, 255.0f / 256, 162.0f / 256),
                    "handcore") {}
    void ActorInput(const pose::InputState& state) {
        HandsBase::ActorInput(state, pose::InputState::KEYPOINT_LEFT_WRIST);
    }
};
