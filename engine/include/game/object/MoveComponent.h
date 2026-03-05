#pragma once
#include "Component.h"
#include "core/Math.h"
#include "document.h"

// 相対空間，絶対空間のどちらにも対応しているが，処理を簡略化するためのお互いの整合性は取っていない
// つまり，相対空間方向の軸にspeedを加えても，自動で絶対空間のspeedに値が加わらない．
class MoveComponent : public Component {
    // 相対空間
    float mForwardSpeed;
    float mRightSpeed;
    float mUpSpeed;
    // 振り向き角速度
    float mYawSpeed;
    float mPitchSpeed;
    float mRollSpeed;

    // 絶対空間
    float mXSpeed;
    float mYSpeed;
    float mZSpeed;
    // 振り向き角速度
    float mXAngularSpeed;
    float mYAngularSpeed;
    float mZAngularSpeed;

    // ownerのpositonを変更する補助関数
    void SetOwnerPosition(const class Vector3& direction, float speed,
                          float deltatime);
    // ownerのrotationを変更する補助関数
    void SetOwnerRotation(const class Vector3& axis, float speed,
                          float deltatime);

   public:
    MoveComponent(class Actor* owner, int updateOrder = 10);
    void Update(float deltatime);

    float GetForwardSpeed() const { return mForwardSpeed; }
    float GetRightSpeed() const { return mRightSpeed; }
    float GetUpSpeed() const { return mUpSpeed; }
    float GetYawSpeed() const { return mYawSpeed; }
    float GetPitchSpeed() const { return mPitchSpeed; }
    float GetRollSpeed() const { return mRollSpeed; }

    float GetXSpeed() const { return mXSpeed; }
    float GetYSpeed() const { return mYSpeed; }
    float GetZSpeed() const { return mZSpeed; }
    float GetXAngularSpeed() const { return mXAngularSpeed; }
    float GetYAngularSpeed() const { return mYAngularSpeed; }
    float GetZAngularSpeed() const { return mZAngularSpeed; }

    void SetForwardSpeed(float forwardSpeed) { mForwardSpeed = forwardSpeed; }
    void SetRightSpeed(float rightSpeed) { mRightSpeed = rightSpeed; }
    void SetUpSpeed(float upSpeed) { mUpSpeed = upSpeed; }
    void SetYawSpeed(float yawSpeed) { mYawSpeed = yawSpeed; }
    void SetPitchSpeed(float pitchSpeed) { mPitchSpeed = pitchSpeed; }
    void SetRollSpeed(float rollSpeed) { mRollSpeed = rollSpeed; }

    void SetXSpeed(float xSpeed) { mXSpeed = xSpeed; }
    void SetYSpeed(float ySpeed) { mYSpeed = ySpeed; }
    void SetZSpeed(float zSpeed) { mZSpeed = zSpeed; }
    void SetXAngularSpeed(float xAngularSpeed) {
        mXAngularSpeed = xAngularSpeed;
    }
    void SetYAngularSpeed(float yAngularSpeed) {
        mYAngularSpeed = yAngularSpeed;
    }
    void SetZAngularSpeed(float zAngularSpeed) {
        mZAngularSpeed = zAngularSpeed;
    }

    // TypeID GetType() const { return TypeID::TMoveComponent; }

    // void LoadProperties(const rapidjson::Value& inObj) override;
};
