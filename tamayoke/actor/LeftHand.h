#pragma once
#include "HandsBase.h"

class LeftHand : public HandsBase {
   public:
    LeftHand(class ActorsSystem* actorSystem, class HandsBaseDeps& deps)
        : HandsBase(actorSystem, deps, 101) {}
    void ActorInput(const pose::InputState& state) {
        HandsBase::ActorInput(state, pose::InputState::KEYPOINT_LEFT_WRIST);
    }
};
