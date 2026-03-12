#pragma once
#include "Component.h"

template <typename T>
class IComponent : public Component {
    static_assert(std::is_base_of<InputStateBase, T>::value,
                  "InputState must derive from InputStateBase");

   public:
    IComponent(class Actor* owner, int updateOrder = 100)
        : Component(owner, updateOrder) {};
    virtual void ProcessInput(const T& state) = 0;
    void IProcessInput(const InputStateBase& state) final {
        ProcessInput(static_cast<T>(state));
    }
};
