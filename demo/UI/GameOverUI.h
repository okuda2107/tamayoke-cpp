#pragma once
#include "BonfireUI.h"

class GameOverUI : public BonfireUI {
   public:
    GameOverUI(class UISystem* system, BonfireUIDeps deps);

    void ProcessInput(const InputState& state) override;

    void SetTime(int time);
};
