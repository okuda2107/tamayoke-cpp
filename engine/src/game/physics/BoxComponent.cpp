#include "game/physics/BoxComponent.h"

#include "game/object/Actor.h"
#include "game/physics/PhysWorld.h"

BoxComponent::BoxComponent(Actor* owner, const std::string& tag,
                           CollisionCompDeps deps, int updateOrder)
    : CollisionComponent(owner, tag, deps, updateOrder),
      mObjectBox(Vector3::Zero, Vector3::Zero),
      mWorldBox(Vector3::Zero, Vector3::Zero),
      mShouldRotate(true) {
    mPhysWorld.AddBox(tag, this);
}

BoxComponent::~BoxComponent() { mPhysWorld.RemoveBox(mTag, this); }

void BoxComponent::OnUpdateWorldTransform() {
    // Reset to object space box
    mWorldBox = mObjectBox;
    // Scale
    mWorldBox.mMin *= mOwner->GetScale();
    mWorldBox.mMax *= mOwner->GetScale();
    // Rotate (if we want to)
    if (mShouldRotate) {
        mWorldBox.Rotate(mOwner->GetRotation());
    }
    // Translate
    mWorldBox.mMin += mOwner->GetPosition();
    mWorldBox.mMax += mOwner->GetPosition();
}
