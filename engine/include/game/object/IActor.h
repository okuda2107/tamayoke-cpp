#pragma once
#include "Actor.h"

template <typename T>
class IActor : public Actor {
    static_assert(std::is_base_of<InputStateBase, T>::value,
                  "InputState must derive from InputStateBase");

   public:
    virtual void ActorInput(const T& state) = 0;
    void IActorInput(const InputStateBase& state) {
        ActorInput(static_cast<T>(state));
    }
};
