#pragma once

class InputProcessorBase {
   public:
    virtual void IProcessInput(const struct InputStateBase& state) = 0;
};

template <typename T>
class InputProcessor : public InputProcessorBase {
   public:
    virtual void ProcessInput(const T& state) = 0;
    void IProcessInput(const struct InputStateBase& state) final {
        ProcessInput(static_cast<const T&>(state));
    }
};
