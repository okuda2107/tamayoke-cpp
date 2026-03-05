#include "game/camera/FPSComponent.h"

#include "core/Math.h"
#include "game/audio/AudioSystem.h"
#include "game/object/Actor.h"

FPSComponent::FPSComponent(class Actor* owner, struct CameraCompDeps ccd,
                           int updateOrder)
    : CameraComponent(owner, ccd, updateOrder),
      mPitch(0.0f),
      mMaxPitch(Math::Pi / 3.0f),
      mPitchSpeed(0.0f) {
    // mOwner->GetGame()->GetInputSystem()->SetRelativeMouseMode(true);
}

FPSComponent::~FPSComponent() {}

void FPSComponent::Update(float deltatime) {
    CameraComponent::Update(deltatime);
    // カメラの位置は所有アクターの位置
    Vector3 cameraPos = mOwner->GetPosition();

    // pitchの角速度に基づいてpitchを更新
    if (!Math::NearZero(mPitchSpeed)) mPitch += mPitchSpeed * deltatime;
    // pitchを最大角度の範囲に収める
    mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);
    // pitch回転を表すクォータニオン
    Quaternion q(mOwner->GetRight(), mPitch);

    // 所有アクターの前方ベクトルを回転
    Vector3 viewForward = Vector3::Transform(mOwner->GetForward(), q);
    // targetの位置は所有アクターの前方100単位
    Vector3 target = cameraPos + viewForward * 100.0f;

    // 上方ベクトルを回転
    Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

    // 注視行列を作って，viewに設定
    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
    SetViewMatrix(view);
}
