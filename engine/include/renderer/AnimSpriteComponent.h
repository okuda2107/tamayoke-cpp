#pragma once
#include <vector>

#include "SpriteComponent.h"

class AnimSpriteComponent : public SpriteComponent {
   protected:
    // 1秒間に何枚表示するか
    int mFPS;
    float mTime;
    std::vector<class Texture*> mTextures;

   public:
    AnimSpriteComponent(class Actor* owner, class RenderDB* database);

    void Update(float deltatime) override;

    void SetTexture(size_t index, class Texture* tex);

    void SetFPS(int fps) { mFPS = fps; }
    // 時間を初期化する際に使用
    void SetTime(float time) { mTime = time; };
};
