#include "game/physics/SphereComponent.h"

#include "game/object/Actor.h"
#include "game/physics/PhysWorld.h"

SphereComponent::SphereComponent(Actor* owner, const std::string& tag,
                                 CollisionCompDeps deps, int updateOrder)
    : CollisionComponent(owner, tag, deps, updateOrder) {
    mPhysWorld.AddSphere(tag, this);
}

SphereComponent::~SphereComponent() { mPhysWorld.RemoveSphere(mTag, this); }

void SphereComponent::OnUpdateWorldTransform() {
    mSphere.mCenter = mOwner->GetPosition();
}
