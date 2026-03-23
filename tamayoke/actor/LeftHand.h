#pragma once
#include "HandsBase.h"

class LeftHand : public HandsBase {
   public:
    LeftHand(class ActorsSystem* actorSystem, class HandsBaseDeps& deps)
        : HandsBase(actorSystem, deps, 101, 100,
                    Vector3(233.0f / 256, 231.0f / 256, 122.0f / 256), "hand") {
    }
    void ActorInput(const pose::InputState& state) {
        HandsBase::ActorInput(state, pose::InputState::KEYPOINT_LEFT_WRIST);
    }
};
