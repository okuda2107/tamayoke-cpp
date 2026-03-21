#pragma once
#include "Actor.h"

template <typename T>
class IActor : public Actor {
    static_assert(std::is_base_of<InputStateBase, T>::value,
                  "InputState must derive from InputStateBase");

   protected:
    virtual void ActorInput(const T& state) = 0;
    void IActorInput(const InputStateBase& state) override {
        // T& の参照型にしないとバグる
        // Tだと新たにオブジェクトを作って，そこにInputStateBaseの値をはめ込むみたいな操作になる
        ActorInput(static_cast<const T&>(state));
    }

   public:
    IActor(class ActorsSystem* system, ActorDeps deps) : Actor(system, deps) {};
};
