#pragma once
#include "Collision.h"
#include "CollisionComponent.h"
#include "game/object/Component.h"

class BoxComponent : public CollisionComponent {
    AABB mObjectBox;
    AABB mWorldBox;
    bool mShouldRotate;

   public:
    BoxComponent(class Actor* owner, const std::string& tag,
                 CollisionCompDeps deps, int updateOrder = 100);
    ~BoxComponent();

    void OnUpdateWorldTransform() override;

    void SetObjectBox(const AABB& model) { mObjectBox = model; }
    const AABB& GetWorldBox() const { return mWorldBox; }

    void SetShouldRotate(bool value) { mShouldRotate = value; }
};
