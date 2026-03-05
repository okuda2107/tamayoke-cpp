#pragma once

#include "BonfireUI.h"
#include "renderer/Shader.h"

class AddWoodUI : public BonfireUI {
   public:
    AddWoodUI(class UISystem* system, BonfireUIDeps deps)
        : BonfireUI(system, deps) {
        SetTitle("Add wood ... [e]");
    }

    void Draw(Shader* shader) override {
        mTitlePos = Vector2(200.0f, 300.0f);
        BonfireUI::Draw(shader);
    }
};
