#pragma once
#include "game/object/Component.h"

// CameraComponentDependencies: CameraComponentの依存関係を束ねたもの
struct CameraCompDeps {
    class RenderDB& renderDB;
    class AudioComponent& audioComponent;

    CameraCompDeps(class RenderDB& renderDB, class AudioComponent& audio)
        : renderDB(renderDB), audioComponent(audio) {}
};

// ゲームオブジェクトに関連付けられたカメラ
class CameraComponent : public Component {
   protected:
    // システムを直接持つのはあまりよくないが，viewを設定する機能を持ったComponentをわざわざ定義するのはめんどくさいので，直接システムを持ちます．
    class RenderDB& mRenderDB;
    class AudioComponent& mAudioComponent;

    void SetViewMatrix(const class Matrix4& view);

   public:
    CameraComponent(class Actor* owner, struct CameraCompDeps& ccd,
                    int drawOrder = 200);
    ~CameraComponent();
};
