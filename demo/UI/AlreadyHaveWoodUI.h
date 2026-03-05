#pragma once

#include "../Utility/Coroutine.h"
#include "BonfireUI.h"

class AlreadyHaveWoodUI : public BonfireUI {
    Coroutine* mCoroutine;
    float endTime;

   public:
    AlreadyHaveWoodUI(class UISystem* system, BonfireUIDeps deps)
        : BonfireUI(system, deps) {
        mCoroutine = new Coroutine();
        endTime = mCoroutine->AddCoroutine(1.0f, [](float t) {});
        SetTitle("already have it ...");
    }
    ~AlreadyHaveWoodUI() { delete mCoroutine; }

    void Update(float deltatime) {
        mCoroutine->Update(deltatime);
        if (mCoroutine->CheckCoroutine(endTime)) {
            Close();
        }
    }

    void Draw(Shader* shader) {
        mTitlePos = Vector2(0.0f, 200.0f);
        BonfireUI::Draw(shader);
    }
};
