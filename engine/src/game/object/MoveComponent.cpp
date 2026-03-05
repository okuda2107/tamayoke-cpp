#include "game/object/MoveComponent.h"

#include "game/object/Actor.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
    : Component(owner, updateOrder),
      mForwardSpeed(0.0f),
      mRightSpeed(0.0f),
      mUpSpeed(0.0f),
      mXSpeed(0.0f),
      mYSpeed(0.0f),
      mZSpeed(0.0f),
      mYawSpeed(0.0f),
      mPitchSpeed(0.0f),
      mRollSpeed(0.0f),
      mXAngularSpeed(0.0f),
      mYAngularSpeed(0.0f),
      mZAngularSpeed(0.0f) {}

void MoveComponent::SetOwnerPosition(const class Vector3& direction,
                                     float speed, float deltatime) {
    Vector3 pos = mOwner->GetPosition();
    pos += direction * speed * deltatime;
    mOwner->SetPosition(pos);
}

void MoveComponent::SetOwnerRotation(const class Vector3& axis, float speed,
                                     float deltatime) {
    Quaternion rot = mOwner->GetRotation();
    float angle = speed * deltatime;
    Quaternion inc(axis, angle);
    rot = Quaternion::Concatenate(rot, inc);
    mOwner->SetRotation(rot);
}

void MoveComponent::Update(float deltatime) {
    if (!Math::NearZero(mForwardSpeed))
        SetOwnerPosition(mOwner->GetForward(), mForwardSpeed, deltatime);
    if (!Math::NearZero(mRightSpeed))
        SetOwnerPosition(mOwner->GetRight(), mRightSpeed, deltatime);
    if (!Math::NearZero(mUpSpeed))
        SetOwnerPosition(mOwner->GetUp(), mUpSpeed, deltatime);
    if (!Math::NearZero(mYawSpeed))
        SetOwnerRotation(mOwner->GetUp(), mYawSpeed, deltatime);
    if (!Math::NearZero(mPitchSpeed))
        SetOwnerRotation(mOwner->GetRight(), mPitchSpeed, deltatime);
    if (!Math::NearZero(mRollSpeed))
        SetOwnerRotation(mOwner->GetForward(), mRollSpeed, deltatime);

    if (!Math::NearZero(mXSpeed))
        SetOwnerPosition(Vector3::UnitX, mXSpeed, deltatime);
    if (!Math::NearZero(mYSpeed))
        SetOwnerPosition(Vector3::UnitY, mYSpeed, deltatime);
    if (!Math::NearZero(mZSpeed))
        SetOwnerPosition(Vector3::UnitZ, mZSpeed, deltatime);
    if (!Math::NearZero(mXAngularSpeed))
        SetOwnerRotation(Vector3::UnitX, mXAngularSpeed, deltatime);
    if (!Math::NearZero(mYAngularSpeed))
        SetOwnerRotation(Vector3::UnitY, mYAngularSpeed, deltatime);
    if (!Math::NearZero(mZAngularSpeed))
        SetOwnerRotation(Vector3::UnitZ, mZAngularSpeed, deltatime);
}

// void MoveComponent::LoadProperties(const rapidjson::Value& inObj) {}
