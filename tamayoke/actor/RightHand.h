#pragma once
#include "HandsBase.h"

class RightHand : public HandsBase {
   public:
    RightHand(class ActorsSystem* actorSystem, class HandsBaseDeps& deps)
        : HandsBase(actorSystem, deps, 100, 100,
                    Vector3(233.0f / 256, 231.0f / 256, 122.0f / 256), "hand") {
    }
    void ActorInput(const pose::InputState& state) {
        HandsBase::ActorInput(state, pose::InputState::KEYPOINT_RIGHT_WRIST);
    }
};
