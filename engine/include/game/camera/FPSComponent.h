#pragma once
#include "CameraComponent.h"

// FPS視点の位置にカメラをアタッチする
class FPSComponent : public CameraComponent {
    // pitchのみActorに依存していないので，変数を持つ必要がある．
    // 現在のpitch
    float mPitch;
    // pitchの最大角度
    float mMaxPitch;
    // pitchの角速度
    float mPitchSpeed;

   public:
    FPSComponent(class Actor* owner, CameraCompDeps fcd, int drawOrder = 200);
    ~FPSComponent();

    void Update(float deltatime) override;

    float GetPitch() const { return mPitch; }

    void SetPitchSpeed(float pitchSpeed) { mPitchSpeed = pitchSpeed; }
    void SetMaxPitch(float maxPitch) { mMaxPitch = maxPitch; }
    void SetPitch(float pitch) { mPitch = pitch; }

    TypeID GetType() const { return TypeID::TFPSComponent; }
};
