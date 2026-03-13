#pragma once
#include "renderer/AnimSpriteComponent.h"

// ボタン用に使いやすくする
class ButtonAnimComp : public AnimSpriteComponent {
   protected:
    // アニメーションを有効にするか
    bool isEnable = false;

   public:
    ButtonAnimComp(class Actor* owner, class RenderDB* database)
        : AnimSpriteComponent(owner, database) {
        //
    }

    void Update(float deltatime) override {
        if (mTextures.empty()) return;
        if (!isEnable) {
            mTexture = mTextures[0];
            mTime = 0;
        } else
            AnimSpriteComponent::Update(deltatime);
    }

    void SetEnable(float flag) { isEnable = flag; }

    // 何秒間接触しているか
    float GetTime() { return mTime; }
    bool GetEnableFlag() { return isEnable; }
};
