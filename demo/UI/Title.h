#pragma once
#include "BonfireUI.h"

class Title : public BonfireUI {
   public:
    Title(class UISystem* system, BonfireUIDeps deps);
    ~Title();

    void ProcessInput(const class InputState& state) override;
};
