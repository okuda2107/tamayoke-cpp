#pragma once

#include "BonfireUI.h"
#include "renderer/Shader.h"

class HaveWoodUI : public BonfireUI {
   public:
    HaveWoodUI(class UISystem* system, BonfireUIDeps deps)
        : BonfireUI(system, deps) {
        SetTitle("pick up wood by press e ...");
    }

    void Draw(Shader* shader) {
        mTitlePos = Vector2(100.0f, 300.0f);
        BonfireUI::Draw(shader);
    }
};
