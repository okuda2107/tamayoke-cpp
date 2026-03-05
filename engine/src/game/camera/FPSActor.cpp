#include "game/camera/FPSActor.h"

#include "game/audio/AudioComponent.h"
#include "game/audio/SoundHandler.h"
#include "game/camera/FPSComponent.h"
#include "game/object/MoveComponent.h"
#include "input/InputState.h"

FPSActor::FPSActor(class ActorsSystem* system, FPSActorDeps fad)
    : Actor(system, fad), mForwardSpeed(0.0f), mStrafeSpeed(0.0f) {
    mMoveComp = new MoveComponent(this);
    mAudioComp = new AudioComponent(this, AudioCompDeps(fad.audioSystem));
    mLastFootstep = 0.0f;
    // mAudioComp->RegisterEvent("footstep");
    // mFootstep = mAudioComp->GetEvent("footstep");
    // mFootstep->SetPaused(true);
    mFootstep = nullptr;
    mFPSComp =
        new FPSComponent(this, CameraCompDeps(fad.renderDB, *mAudioComp));
}

void FPSActor::ActorInput(const InputState& state) {
    float fowardSpeed = 0.0f;
    if (state.Keyboard.GetKeyValue(mForwardKey)) fowardSpeed += mForwardSpeed;
    if (state.Keyboard.GetKeyValue(mBackKey)) fowardSpeed -= mForwardSpeed;
    mMoveComp->SetForwardSpeed(fowardSpeed);

    float rightSpeed = 0.0f;
    if (state.Keyboard.GetKeyValue(mRightKey)) rightSpeed += mStrafeSpeed;
    if (state.Keyboard.GetKeyValue(mLeftKey)) rightSpeed -= mStrafeSpeed;
    mMoveComp->SetRightSpeed(rightSpeed);

    Vector2 mousePos = state.Mouse.GetPostion();

    float yawSpeed = 0;
    if (!Math::NearZero(mousePos.x)) {
        yawSpeed = static_cast<float>(mousePos.x) / mMaxMouseSpeed;
        yawSpeed *= mMaxAngularSpeed;
    }
    mMoveComp->SetYawSpeed(yawSpeed);

    float pitchSpeed = 0;
    if (!Math::NearZero(mousePos.y)) {
        pitchSpeed = static_cast<float>(mousePos.y) / mMaxMouseSpeed;
        pitchSpeed *= mMaxAngularSpeed;
    }
    mFPSComp->SetPitchSpeed(pitchSpeed);
}

void FPSActor::UpdateActor(float deltaTime) {
    Actor::UpdateActor(deltaTime);

    // Play the footstep if we're moving and haven't recently
    mLastFootstep -= deltaTime;
    if (mFootstep != nullptr && !Math::NearZero(mMoveComp->GetForwardSpeed()) &&
        mLastFootstep <= 0.0f) {
        mFootstep->SetPaused(false);
        mFootstep->Restart();
        mLastFootstep = 0.5f;
    }

    // Update position of FPS model relative to actor position
    const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
    Vector3 modelPos = GetPosition();
    modelPos += GetForward() * modelOffset.x;
    modelPos += GetRight() * modelOffset.y;
    modelPos.z += modelOffset.z;
}
